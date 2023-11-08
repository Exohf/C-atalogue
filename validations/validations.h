#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "utils.h"
#include "../database/database_functions.h"

#define REGEX_IPV4_VALIDATION "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"

int verifyIPAddress(char *ipAddress);
void askAndValidateMask(char *mask);
int validateMask(char *mask);
void getAndVerifyIPAddress();

#endif