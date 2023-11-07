#ifndef DATABASE_DATABASE_FUNCTIONS_H
#define DATABASE_DATABASE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

sqlite3 *openDatabaseConnection(const char *db_name);
void closeDatabaseConnection(sqlite3 *db);
int initializeDatabase(const char *db_name);
void listTableContents(const char *db_name);

#endif /* DATABASE_DATABASE_FUNCTIONS_H */