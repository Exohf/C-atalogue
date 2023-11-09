#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "database_functions.h"

static sqlite3 *db = NULL;

sqlite3 *openDatabaseConnection(const char *db_name)
{
    if (db)
    {
        fprintf(stdout, "Database connection already open\n");
        return db;
    }

    int return_code = sqlite3_open(db_name, &db);
    if (return_code != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        fprintf(stdout, "\nOpened database successfully\n");
        return db;
    }
}

void closeDatabaseConnection()
{
    if (db)
    {
        sqlite3_close(db);
        fprintf(stdout, "\nClosed database successfully\n");
        db = NULL;
    }
    else
    {
        fprintf(stderr, "\nNo open database connection to close\n");
    }
}

void addIpToDatabase(const char *ip_address, const char *mask, const char *db_name)
{
    if (!db)
    {
        openDatabaseConnection(db_name);
    }

    char sql[100];
    snprintf(sql, sizeof(sql), "INSERT INTO IP_ADDRESS_LIST(ip_address, subnet_mask) VALUES ('%s', '%s')", ip_address, mask);
    sqlite3_exec(db, sql, 0, 0, 0);
    printf("Adresse IP ajoutée avec succès !\n");
    closeDatabaseConnection(db_name);
}

void listAllEntries(const char *db_name)
{
    sqlite3 *db = openDatabaseConnection(db_name);

    if (!db)
    {
        fprintf(stderr, "Failed to open the database\n");
        return;
    }

    char *sql = "SELECT * FROM IP_ADDRESS_LIST;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to execute the query: %s\n", sqlite3_errmsg(db));
        closeDatabaseConnection();
        return;
    }

    printf("List of all entries in the database:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("ID = %d\n", sqlite3_column_int(stmt, 0));
        printf("IP Address = %s\n", sqlite3_column_text(stmt, 1));
        printf("Subnet Mask = %s\n", sqlite3_column_text(stmt, 2));
        printf("\n");
    }

    sqlite3_finalize(stmt);
    closeDatabaseConnection();
}

int initializeDatabase(const char *db_name)
{
    if (!openDatabaseConnection(db_name))
    {
        fprintf(stderr, "Failed to open the database\n");
        return 1;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS IP_ADDRESS_LIST(Id INTEGER PRIMARY KEY AUTOINCREMENT, ip_address TEXT NOT NULL, subnet_mask TEXT NOT NULL);";

    char *err_msg = 0;
    int return_code = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (return_code != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        closeDatabaseConnection();
        return 1;
    }
    else
    {
        fprintf(stdout, "Table for ip_address found\n");
    }

    return 0;
}