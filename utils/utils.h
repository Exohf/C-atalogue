#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void clearConsole();
void warnHelpCommand();
void handleInvalidCommand();
void clearInputBuffer();
void start();
void displayHelper();
int verifyIPAddress(char *ipAddress);
void askAndValidateMask(char *mask);
int validateMask(char *mask);
void getAndVerifyIPAddress();

#endif /* UTILS_UTILS_H */
