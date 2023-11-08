#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "../utils/utils.h"
#include "../database/database_functions.h"

#define REGEX_IPV4_VALIDATION "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"

int compileRegex(regex_t *regex)
{
    int compiledRegex = regcomp(regex, REGEX_IPV4_VALIDATION, REG_EXTENDED);
    if (compiledRegex)
    {
        fprintf(stderr, "Could not compile regex\n");
        regfree(regex);
    }
    return compiledRegex;
}

int executeRegex(regex_t *regex, char *ipAddress)
{
    int executedRegex = regexec(regex, ipAddress, 0, NULL, 0);
    return executedRegex;
}

void printIPAddressValidity(char *ipAddress, int isValid)
{
    if (isValid)
    {
        printf("Valid IP Address: %s\n\n", ipAddress);
        clearInputBuffer();
    }
    else
    {
        printf("Invalid IP Address: %s\n", ipAddress);
    }
}

void printRegexError(int reti, regex_t *regex)
{
    char msgbuf[100];
    regerror(reti, regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
}

int verifyIPAddress(char *ipAddress)
{
    regex_t regex;
    int verifyProcess;

    verifyProcess = compileRegex(&regex);
    if (verifyProcess)
    {
        regfree(&regex);
        return -1; // Return -1 to indicate regex compilation failure
    }

    verifyProcess = executeRegex(&regex, ipAddress);
    printIPAddressValidity(ipAddress, verifyProcess == 0);

    regfree(&regex);

    if (verifyProcess == 0)
    {
        return 1; // Return 1 to indicate a valid IP address
    }
    else if (verifyProcess == REG_NOMATCH)
    {
        return 0; // Return 0 to indicate an invalid IP address
    }
    else
    {
        return -1; // Return -1 to indicate regex execution failure
    }
}

int validateMask(char *mask)
{
    regex_t regex;
    int reti;

    // Compile the regular expression
    reti = regcomp(&regex, REGEX_IPV4_VALIDATION, REG_EXTENDED);
    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        return 0; // Return 0 to indicate regex compilation failure
    }

    bool valid_input = false;
    while (!valid_input)
    {
        // Execute the regular expression
        reti = regexec(&regex, mask, 0, NULL, 0);
        if (reti == 0)
        {
            printf("Valid mask: %s\n", mask);
            regfree(&regex);
            clearInputBuffer();
            return 1; // Return 1 to indicate a valid mask
        }
        else if (reti == REG_NOMATCH)
        {
            printf("Invalid mask: %s\n", mask);
            clearInputBuffer();
        }
        else
        {
            char msgbuf[100];
            regerror(reti, &regex, msgbuf, sizeof(msgbuf));
            fprintf(stderr, "Regex match failed: %s\n", msgbuf);
            regfree(&regex);
            return 0; // Return 0 to indicate regex execution failure
        }

        printf("Enter a valid mask for the IP address in the format xxx.xxx.xxx.xxx (or 'q' to quit): ");
        scanf("%s", mask);

        if (strcmp(mask, "q") == 0)
        {
            printf("Quitting the program\n");
            break;
        }
    }

    regfree(&regex);
    return 0; // Return 0 in case of unexpected error
}

void askAndValidateMask(char *mask)
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

        if (validateMask(mask))
        {
            valid_mask = true;
            // Add your logic here for using the valid mask
        }
    }
}

void getAndVerifyIPAddress()
{
    char ip_address[16]; // Adjust the size according to your needs
    char mask[16];       // Adjust the size according to your needs
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

        if (verifyIPAddress(ip_address))
        {
            askAndValidateMask(mask);
            valid = true;
            // Add your logic here for using the valid IP address and mask
        }
    }
}