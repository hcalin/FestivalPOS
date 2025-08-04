#ifndef _UI_H_
#define _UI_H_

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "RFID.h"
#include "DB.h"

#define ITEMS_IN_BAR 3U
#define ITEMS_IN_ADMIN 3U

#define BAR_ADMIN_MENU 0U
#define BAR_MENU 1U
#define BAR_CHECKOUT 2U

#define BAL_CHECK_MENU 9U
#define ADMIN_MAIN_MENU 10U
#define ADMIN_MAIN_MENU_GRANTED 11U

extern uint16_t max_row,max_col;
extern uint8_t nuidPICC[4];
//extern int DB_nuid_exists(uint32_t nuid, DB_info_s *DB_info);
struct DB_info_s;
void UI_init();
void UI_start();
void UI_loop();
void UI_print_bar();
void UI_check_bar();
void UI_check_admin();
void UI_print_bar_admin();
void UI_print_checkout();
void UI_print_admin();
void UI_print_check_bal();
void UI_create_back_button();
void UI_clear_back_button();
void UI_create_checkout();
void UI_admin_add_nuid(uint32_t nuid);
void UI_admin_del_nuid(uint32_t nuid);
int UI_draw_numpad(uint32_t nuid, struct DB_info_s DB_info);
void print_center(uint16_t row, char *title);
void wprint_center(WINDOW * win, uint16_t row, char *title);
void wprint_center_box(WINDOW * win, char *title);

#endif
