#include "DB.h"

int DB_init() {
    sqlite3 *db;             // SQLite database connection handle
    char *err_msg = NULL;    // Error message buffer
    int rc;

    // Open the SQLite database file
    rc = sqlite3_open("my_database.db", &db);

    if (rc != SQLITE_OK) {
		print_center((max_row/2) + 1, "Database connection was not successfull.");
        sqlite3_close(db);
        return 1;
    }

    print_center((max_row/2) + 1, "Database connection was  successfull.");
    refresh();
    
    // Execute a simple SQL query
    const char *sql = "SELECT id, name, age FROM users";
    sqlite3_stmt *stmt;
/*
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Print query results
    printf("ID | Name    | Age\n");
    printf("--------------------\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);

        printf("%d  | %s | %d\n", id, name, age);
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
*/
    return 0;
}
