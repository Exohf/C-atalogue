#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void clearConsole();
void warnHelpCommand();
void handleInvalidCommand();
void clearInputBuffer();
void start();
void displayHelper();

#endif /* UTILS_UTILS_H */
