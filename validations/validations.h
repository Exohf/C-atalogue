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

void printNoCompilationRegex();
int compileAndPrintErrorRegex(regex_t *regex);
void printAddressValidity(char *address, int isValid, int isIp);
void printRegexError(int failedProcess, regex_t *regex);
int handleVerificationResult(int verifyProcess, regex_t *regex);
int verifyAddress(char *address, int isIp);
void getAndVerifyIpMask(char *mask);
void getAndVerifyIPAddress();

#endif /* VALIDATIONS_H */