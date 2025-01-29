#ifndef _DB_H_
#define _DB_H_

#include "UI.h"
#include <sqlite3.h> 

int DB_init();

extern void print_center(uint16_t row, char *title);
extern uint16_t max_row;

#endif
