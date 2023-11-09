#ifndef DATABASE_DATABASE_FUNCTIONS_H
#define DATABASE_DATABASE_FUNCTIONS_H

#include <sqlite3.h>

sqlite3 *openDatabaseConnection(const char *db_name);
void closeDatabaseConnection();
void addIpToDatabase(const char *ip_address, const char *mask, const char *db_name);
void listAllEntries(const char *db_name);
int initializeDatabase(const char *db_name);

#endif /* DATABASE_DATABASE_FUNCTIONS_H */