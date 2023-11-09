#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "../utils/utils.h"
#include "../database/database_functions.h"

#define REGEX_IPV4_VALIDATION "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"

void printNoCompilationRegex()
{
    fprintf(stderr, "Could not compile regex\n");
}

int compileAndPrintErrorRegex(regex_t *regex)
{
    int compiledRegex = regcomp(regex, REGEX_IPV4_VALIDATION, REG_EXTENDED);
    if (compiledRegex)
    {
        printNoCompilationRegex();
        regfree(regex);
    }
    return compiledRegex;
}

void printAddressValidity(char *address, int isValid, int isIp)
{
    if (isIp)
    {
        isValid ? printf("Valid IP Address: %s\n", address) : printf("Invalid IP Address %s\n", address);
    }
    else
    {
        isValid ? printf("Valid mask address: %s\n", address) : printf("Invalid mask address: %s\n", address);
    }
}

void printRegexError(int failedProcess, regex_t *regex)
{
    char msgbuf[100];
    regerror(failedProcess, regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
}

int handleVerificationResult(int verifyProcess, regex_t *regex)
{
    if (verifyProcess == 0)
    {
        // Match
        regfree(regex);
        return 1;
    }
    else if (verifyProcess == REG_NOMATCH)
    {
        return 0;
    }
    else
    {
        // Error
        regfree(regex);
        printRegexError(verifyProcess, regex);
        return -1;
    }
}

int verifyAddress(char *address, int isIp)
{
    regex_t regex;
    int verifyProcess;

    if (compileAndPrintErrorRegex(&regex))
        return 0;

    verifyProcess = regexec(&regex, address, 0, NULL, 0);
    printAddressValidity(address, verifyProcess == 0, isIp);

    return handleVerificationResult(verifyProcess, &regex);
}

void getAndVerifyIpMask(char *mask)
{
    bool valid_mask = false;

    while (!valid_mask)
    {
        printf("Enter a mask for the IP address in the format xxx.xxx.xxx.xxx: ");
        scanf("%s", mask);

        if (strcmp(mask, "q") == 0)
        {
            printf("Quitting the program\n");
            clearInputBuffer();
            break;
        }

        if (verifyAddress(mask, 0))
        {
            clearInputBuffer();
            valid_mask = true;
        }
    }
}

void getAndVerifyIPAddress(const char *db_name)
{
    char ip_address[16];
    char mask[16];
    bool valid = false;

    while (!valid)
    {
        printf("Enter an IP address (or 'q' to quit): ");
        scanf("%s", ip_address);

        if (strcmp(ip_address, "q") == 0)
        {
            printf("Quitting the program\n");
            clearInputBuffer();
            break;
        }

        if (verifyAddress(ip_address, 1))
        {
            getAndVerifyIpMask(mask);
            valid = true;
        }
    }

    if (valid)
        addIpToDatabase(ip_address, mask, db_name);
    // printf("%s, %s", ip_address, mask);
}