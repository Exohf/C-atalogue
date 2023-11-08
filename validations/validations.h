#ifndef VALIDATIONS_VALIDATIONS_H
#define VALIDATIONS_VALIDATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "../utils/utils.h"
#include "../database/database_functions.h"

#define REGEX_IPV4_VALIDATION "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"

int compileRegex(regex_t *regex);
int executeRegex(regex_t *regex, char *ipAddress);
void printIPAddressValidity(char *ipAddress, int isValid);
void printRegexError(int reti, regex_t *regex);
int verifyIPAddress(char *ipAddress);
int validateMask(char *mask);
void askAndValidateMask(char *mask);
void getAndVerifyIPAddress();

#endif /* VALIDATIONS_H */