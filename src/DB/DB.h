#ifndef _DB_H_
#define _DB_H_

#include "UI.h"
#include <sqlite3.h> 

struct DB_info_s
{
	const unsigned char *name;
	int balance;
	const unsigned char *date_created;
};

int DB_init();
int DB_nuid_exists(uint32_t nuid, DB_info_s *DB_info);

extern void print_center(uint16_t row, char *title);
extern uint16_t max_row;

#endif
