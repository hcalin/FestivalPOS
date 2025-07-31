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
    sqlite3_exec(db,"PRAGMA journal_mode = WAL", NULL, NULL, NULL);
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

int DB_check_balance(uint32_t nuid, DB_info_s *DB_info, uint16_t total)
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
        
        if (DB_info->balance > total)
        { 
            return 1;
        }
    }
    else
    {
        // Not found in the database
    }
    
    sqlite3_finalize(stmt);
    return 0;
}

int DB_modify_balance(int32_t nuid, double amount)
{
    int rc;
    // Execute a simple SQL query
    const char *sql = "UPDATE users SET sold = ? WHERE RFID = ?;";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return -1; // Failed to prepare statement
    }
    
    sqlite3_bind_double(stmt, 1,  amount);
    sqlite3_bind_int(stmt, 2 ,nuid);
    
    sqlite3_exec(db,"COMMIT;",NULL, NULL, NULL);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return rc; 
    }
    
    int changes = sqlite3_changes(db);
    if(changes == 0)
    {
        return 2;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}

int DB_update_payments(int32_t nuid, double amount)
{
    int rc;
    // Execute a simple SQL query
    const char *sql = "INSERT INTO payments (RFID, amount, beer, wine, palinka) VALUES (?,?,?,?,?)";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return -1; // Failed to prepare statement
    }
    
    sqlite3_bind_int(stmt, 1 ,nuid);
    sqlite3_bind_double(stmt, 2,  amount);
    
    for(size_t idx = 0 ; idx < ITEMS_IN_BAR ; idx++)
    {
        sqlite3_bind_int(stmt, idx + 3, bar_quantity[idx]);
    }
    
    
    
    sqlite3_exec(db,"COMMIT;",NULL, NULL, NULL);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return rc; 
    }
    
    int changes = sqlite3_changes(db);
    if(changes == 0)
    {
        return 2;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}

int DB_add_user(int32_t nuid)
{
    int rc;
    // Execute a simple SQL query
    const char *sql = "INSERT INTO users (rfid,name,sold) VALUES (?,?,?);";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return -1; // Failed to prepare statement
    }
    
    const char *s = "John doe";
    sqlite3_bind_int(stmt, 1 ,nuid);
    sqlite3_bind_text(stmt, 2, s, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, 0.0f);
    sqlite3_exec(db,"COMMIT;",NULL, NULL, NULL);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return rc; 
    }
    
    int changes = sqlite3_changes(db);
    if(changes == 0)
    {
        return 2;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}

int DB_delete_user(int32_t nuid)
{
    int rc;
    // Execute a simple SQL query
    const char *sql = "DELETE FROM users WHERE rfid = ?;";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return -1; // Failed to prepare statement
    }
    
    sqlite3_bind_int(stmt, 1 ,nuid);
    sqlite3_exec(db,"COMMIT;",NULL, NULL, NULL);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return rc; 
    }
    
    int changes = sqlite3_changes(db);
    if(changes == 0)
    {
        return 2;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}

int DB_add_sold(int32_t nuid, double amount)
{
    int rc;
    // Execute a simple SQL query
    const char *sql = "INSERT INTO payments (RFID, amount, beer, wine, palinka) VALUES (?,?,?,?,?)";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return -1; // Failed to prepare statement
    }
    
    sqlite3_bind_int(stmt, 1 ,nuid);
    sqlite3_bind_double(stmt, 2,  amount);
    
    for(size_t idx = 0 ; idx < ITEMS_IN_BAR ; idx++)
    {
        sqlite3_bind_int(stmt, idx + 3, bar_quantity[idx]);
    }
    
    
    
    sqlite3_exec(db,"COMMIT;",NULL, NULL, NULL);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return rc; 
    }
    
    int changes = sqlite3_changes(db);
    if(changes == 0)
    {
        return 2;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}
