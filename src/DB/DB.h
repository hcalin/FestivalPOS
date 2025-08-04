#ifndef _DB_H_
#define _DB_H_

#include "UI.h"
#include <sqlite3.h> 

struct DB_info_s
{
	const unsigned char *name;
	double balance;
	const unsigned char *date_created;
	int isAdmin;
};

int DB_init();
int DB_nuid_exists(uint32_t nuid, DB_info_s *DB_info);
int DB_check_balance(uint32_t nuid, DB_info_s *DB_info, uint16_t total);
int DB_modify_balance(int32_t nuid, double amount);
int DB_update_payments(int32_t nuid, double amount);
int DB_add_user(int32_t nuid);
int DB_delete_user(int32_t nuid);
int DB_add_sold(int32_t nuid, double amount);
int DB_check_admin(uint32_t nuid, DB_info_s *DB_info);

extern void print_center(uint16_t row, char *title);
extern uint16_t max_row;
extern uint8_t bar_quantity[3];

#endif
