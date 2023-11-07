#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "database_functions.h"


sqlite3 *openDatabaseConnection(const char *db_name)
{
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc)
    {
        fprintf(stderr, "\nCan't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        fprintf(stdout, "\nOpened database successfully\n");
        return db;
    }
}

void closeDatabaseConnection(sqlite3 *db)
{
    if (db)
    {
        sqlite3_close(db);
        fprintf(stdout, "Closed database successfully\n");
    }
    else
    {
        fprintf(stderr, "No open database connection to close\n");
    }
}

int initializeDatabase(const char *db_name)
{
    sqlite3 *db = openDatabaseConnection(db_name);

    if (!db)
    {
        fprintf(stderr, "Failed to open the database\n");
        return 1;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS IP_ADDRESS_LIST(Id INT, Ip_address TEXT, Mask TEXT);";

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    else
    {
        fprintf(stdout, "Table for ip_address found\n");
    }

    return 0;
}

void listTableContents(const char *db_name)
{
    sqlite3 *db = openDatabaseConnection(db_name);

    if (!isDatabaseOpened(db_name))
    {
        fprintf(stderr, "Failed to open the database\n");
        return;
    }

    char *sql = "SELECT * FROM IP_ADDRESS_LIST;";
    sqlite3_stmt *res;

    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    fprintf(stdout, "ID\tIP_ADDRESS\tMASK\n");

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        printf("%d\t%s\t%s\n",
               sqlite3_column_int(res, 0),
               sqlite3_column_text(res, 1),
               sqlite3_column_text(res, 2));
    }

    sqlite3_finalize(res);
    closeDatabaseConnection(db);
}