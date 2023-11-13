#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "../utils/utils.h"
#include "../database/database_functions.h"

#define REGEX_IPV4_VALIDATION "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"

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

int isValidSubnetMask(char *subnetMask)
{
    char *subnetMask_copy = strdup(subnetMask);

    if (subnetMask_copy == NULL)
    {
        printAddressValidity(subnetMask, 0, 0);
        return 0; // NULL pointer
    }

    // Validate the dotted-decimal format
    int octet;
    if (sscanf(subnetMask_copy, "%d.%d.%d.%d", &octet, &octet, &octet, &octet) != 4)
    {
        printAddressValidity(subnetMask, 0, 0);
        return 0; // Invalid format
    }

    // Convert dotted-decimal to binary
    unsigned long binaryMask = 0;
    int i = 24;
    char *octetStr;
    octetStr = strtok(subnetMask_copy, ".");
    while (octetStr != NULL)
    {
        int octetValue = atoi(octetStr);
        if (octetValue < 0 || octetValue > 255)
        {
            printAddressValidity(subnetMask, 0, 0);
            return 0; // Invalid octet value
        }
        binaryMask |= (unsigned long)octetValue << i;
        i -= 8;
        octetStr = strtok(NULL, ".");
    }

    // Convert binary to a binary string representation
    char binaryString[33]; // 32 bits + '\0'
    for (int j = 0; j < 32; j++)
    {
        binaryString[j] = ((binaryMask >> (31 - j)) & 1) ? '1' : '0';
    }
    binaryString[32] = '\0'; // Null-terminate the string

    // Check if the binary representation is a valid subnet mask
    int foundZero = 0; // Flag to check for consecutive 0s
    for (int k = 0; k < 32; k++)
    {
        if (binaryString[k] == '1')
        {
            if (foundZero)
            {
                printAddressValidity(subnetMask, 0, 0);
                return 0; // Invalid subnet mask (consecutive 0s)
            }
        }
        else if (binaryString[k] == '0')
        {
            foundZero = 1;
        }
        else
        {
            printAddressValidity(subnetMask, 0, 0);
            return 0; // Invalid character (neither '0' nor '1')
        }
    }
    printAddressValidity(subnetMask, 1, 0);
    return 1; // Valid subnet mask
}

int isValidIPv4(char *ip_address)
{
    // Create a copy of the input string
    char *ip_copy = strdup(ip_address);

    // Tokenize the copied IP address using dots as delimiters
    char *token = strtok(ip_copy, ".");

    // Check if there are exactly four octets
    int octetCount = 0;
    while (token != NULL)
    {
        octetCount++;

        // Convert the octet to an integer
        int octet = atoi(token);

        // Check if the octet is within the valid range (0 to 255)
        if (octet < 0 || octet > 255)
        {
            free(ip_copy); // Free the allocated memory
            printAddressValidity(ip_address, 0, 1);
            return 0; // Invalid octet
        }

        // Check for leading zeros in the octet
        if (strlen(token) > 1 && token[0] == '0')
        {
            free(ip_copy); // Free the allocated memory
            printAddressValidity(ip_address, 0, 1);
            return 0; // Leading zeros are not allowed
        }

        // Move to the next octet
        token = strtok(NULL, ".");
    }

    // Check if there are exactly four octets
    if (octetCount != 4)
    {
        printAddressValidity(ip_address, 0, 1);
        free(ip_copy); // Free the allocated memory
        return 0;      // Invalid number of octets
    }

    printAddressValidity(ip_address, 1, 1);
    free(ip_copy); // Free the allocated memory
    return 1;      // The IP address is valid
}

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

    if (isIp)
    {
        if (!isValidIPv4(address))
        {
            return 0;
        }
    }
    else
    {
        if (!isValidSubnetMask(address))
        {
            return 0;
        }
    }

    return handleVerificationResult(verifyProcess, &regex);
}

int getAndVerifyIpMask(char *mask)
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
            return 0;
        }

        if (verifyAddress(mask, 0))
        {
            clearInputBuffer();
            return 1;
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
            if (getAndVerifyIpMask(mask))
            {
                valid = true;
            }
        }
    }

    if (valid)
        addIpToDatabase(ip_address, mask, db_name);
    // printf("%s, %s", ip_address, mask);
}