#include "UI.h"

extern uint16_t max_row,max_col;


char input_buffer[9] = "________";
char current_sign = '+';
char final_result[10] = "";
uint32_t number_after_ok;

#define NUMPAD_BUTTONS 13

typedef struct {
    int y;
    int x;
    int height;
    int width;
    char *label;
} NumpadButton;


NumpadButton *create_buttons(int height, int width) {
    NumpadButton *buttons = (NumpadButton*)malloc(sizeof(NumpadButton) * 13);
    int numpad_width = 18;
    int numpad_x = width - numpad_width - 2;
    int start_y = height/2 - 6;

    char *labels[] = {"7", "8", "9", "4", "5", "6", 
                      "1", "2", "3", "0", "DEL", "+/-", "OK"};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            int index = row * 3 + col;
            if (index < 12) {
                buttons[index] = (NumpadButton){
                    start_y + row * 3, 
                    numpad_x + col * 6, 
                    3, 5, 
                    strdup(labels[index])
                };
            }
        }
        // Place OK button under DEL
        if (row == 3) {
            buttons[12] = (NumpadButton){
                start_y + row * 3 + 3, 
                numpad_x + 12, 
                3, 5, 
                strdup("OK")
            };
        }
    }

    return buttons;
}

void draw_sign(int height, int width) {
    mvprintw(height/2 - 2, width/2 - 5, "Current Sign: %c", current_sign);
}

void draw_input(int height, int width) {
    mvprintw(height/2 - 1, width/2 - 5, "Input: %s", input_buffer);
}

void draw_numpad(NumpadButton *buttons) {
    for (int i = 0; i < NUMPAD_BUTTONS; i++) {
        mvprintw(buttons[i].y, buttons[i].x, "[%s]", buttons[i].label);
    }
}

void draw_result(int height, int width) {
    mvprintw(height/2 + 4, width/2 - 5, "Result: %s", final_result);
}

int is_mouse_on_button(MEVENT *event, NumpadButton *button) {
    return (event->y >= button->y && event->y < button->y + button->height &&
            event->x >= button->x && event->x < button->x + button->width);
}


void shift_buffer(char mode) {
    if (mode == 'l') {
        for (int i = 0; i < 7; i++) {
            input_buffer[i] = input_buffer[i+1];
        }
        input_buffer[7] = '_';
    }
}

void UI_draw_numpad(uint32_t nuid, DB_info_s DB_info)
{
	int height, width;
    getmaxyx(stdscr, height, width);

    NumpadButton *buttons = create_buttons(height, width);

    draw_sign(height, width);
    draw_input(height, width);
    draw_numpad(buttons);
    draw_result(height, width);
    refresh();

    int ch;
    MEVENT event;
    while ((ch = getch()) != 'q') {
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                for (int i = 0; i < NUMPAD_BUTTONS; i++) {
                    if (is_mouse_on_button(&event, &buttons[i])) {
                        if (strcmp(buttons[i].label, "+/-") == 0) {
                            current_sign = (current_sign == '+') ? '-' : '+';
                        } else if (strcmp(buttons[i].label, "DEL") == 0) {
                            strcpy(input_buffer, "________");
                        } else if (strcmp(buttons[i].label, "OK") == 0) {
                            strcpy(final_result, current_sign == '-' ? "-" : "");
                            char *temp = input_buffer;
                            while(*temp == '_') temp++;
                            
                            number_after_ok = atoi(temp);
                            if(current_sign == '-')
								number_after_ok *= -1;
                            
                            //mvwprintw(stdscr, 1, 2, "%d", number_after_ok);
                            
                            DB_add_sold(nuid, number_after_ok);
                            strcat(final_result, input_buffer);
                            strcpy(input_buffer, "________");
                        } else {
                            shift_buffer('l');
                            input_buffer[7] = buttons[i].label[0];
                        }
                        
                        draw_sign(height, width);
                        draw_input(height, width);
                        draw_numpad(buttons);
                        draw_result(height, width);
                        refresh();
                        break;
                    }
                }
            }
        }
    }

}
