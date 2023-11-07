#ifndef DATABASE_DATABASE_FUNCTIONS_H
#define DATABASE_DATABASE_FUNCTIONS_H

#include <sqlite3.h>

sqlite3 *openDatabaseConnection(const char *db_name);
void closeDatabaseConnection();
int initializeDatabase(const char *db_name);
void listTableContents(const char *db_name);

#endif /* DATABASE_DATABASE_FUNCTIONS_H */