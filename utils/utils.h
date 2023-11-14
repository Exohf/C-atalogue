#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include "../database/database_functions.h"
#include "../validations/validations.h"

void clearConsole();
void warnHelpCommand();
void handleInvalidCommand();
void clearInputBuffer();
void start();
void displayHelper();
char *toBinary(char *subnetMask);
int isNumeric(const char *str);
void deleteEntryMenu(const char *db_name);
void displayBinaryRepresentation(const char *binary);
void searchFromSameNetworkIp(const char *db_name);

#endif /* UTILS_UTILS_H */
