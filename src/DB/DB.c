#include "DB.h"

sqlite3 *db;

int DB_init() {
    // SQLite database connection handle
    char *err_msg = NULL;    // Error message buffer
    int rc;

    // Open the SQLite database file
    rc = sqlite3_open("SLOPE_database.db", &db);

    if (rc != SQLITE_OK) {
		print_center((max_row/2) + 1, "Database connection was not successfull.");
        sqlite3_close(db);
        return 1;
    }

    print_center((max_row/2) + 1, "Database connection was  successfull.");
    refresh();
    
    return 0;
}

int DB_nuid_exists(uint32_t nuid, DB_info_s *DB_info)
{
    // Execute a simple SQL query
    const char *sql = "SELECT Name, sold, date_created FROM Users WHERE RFID = ?;";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
        return -1; // Failed to prepare statement
    }
    
    sqlite3_bind_int(stmt, 1 ,nuid);
    
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        DB_info->name = sqlite3_column_text(stmt, 0);
        DB_info->balance = sqlite3_column_int (stmt, 1);
        DB_info->date_created = sqlite3_column_text(stmt, 2);
        
        return 1;
    }
    else
    {
        // Not found in the database
    }
    
    sqlite3_finalize(stmt);
    return 0;
}
