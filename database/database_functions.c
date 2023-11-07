#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "database_functions.h"

static sqlite3 *db = NULL;

sqlite3 *openDatabaseConnection(const char *db_name) {
    if (db) {
        fprintf(stdout, "Database connection already open\n");
        return db;
    }

    int return_code = sqlite3_open(db_name, &db);
    if (return_code != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    } else {
        fprintf(stdout, "\nOpened database successfully\n");
        return db;
    }
}

void closeDatabaseConnection() {
    if (db) {
        sqlite3_close(db);
        fprintf(stdout, "\nClosed database successfully\n");
        db = NULL;
    } else {
        fprintf(stderr, "\nNo open database connection to close\n");
    }
}

int initializeDatabase(const char *db_name) {
    if (!openDatabaseConnection(db_name)) {
        fprintf(stderr, "Failed to open the database\n");
        return 1;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS IP_ADDRESS_LIST(Id INT, Ip_address TEXT, Mask TEXT);";

    char *err_msg = 0;
    int return_code = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (return_code != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        closeDatabaseConnection();
        return 1;
    } else {
        fprintf(stdout, "Table for ip_address found\n");
    }

    return 0;
}