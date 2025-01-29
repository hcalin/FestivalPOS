#ifndef _UI_H_
#define _UI_H_

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>


#define ITEMS_IN_BAR 3U
#define ITEMS_IN_ADMIN 3U

#define BAR_ADMIN_MENU 0U
#define BAR_MENU 1U
#define BAR_CHECKOUT 2U

#define ADMIN_MAIN_MENU 10U


extern uint16_t max_row,max_col;



void UI_init();
void UI_start();
void UI_loop();
void UI_print_bar();
void UI_check_bar();
void UI_print_bar_admin();
void UI_print_checkout();
void UI_print_admin();
void print_center(uint16_t row, char *title);
void wprint_center(WINDOW * win, uint16_t row, char *title);
void wprint_center_box(WINDOW * win, char *title);

#endif
