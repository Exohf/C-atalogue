#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "utils.h"
#include "../database/database_functions.h"

#define REGEX_IPV4_VALIDATION "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"

void clearConsole()
{
    system("clear");
}

void warnHelpCommand()
{
    printf("\n\ntype 'h' to list commands available\n");
    printf("type 'q' to exit program\n\n");
}

void handleInvalidCommand()
{
    printf("Invalid command. Please try again.\n");
}

void clearInputBuffer()
{
    int d;
    while ((d = getchar()) != '\n' && d != EOF)
        ;
}

void displayHelper()
{

    printf("\n"
           "a - Add a new IP address\n"
           "c - Clear the console\n"
           "l - List IP addresses\n"
           "s - Search similar by mask\n"
           "d - Delete an IP\n"
           "q - Quit the program\n"
           "\n");
}

int verifyIPAddress(char *ipAddress)
{
    regex_t regex;
    int reti;

    // Compile the regular expression
    reti = regcomp(&regex, REGEX_IPV4_VALIDATION, REG_EXTENDED);
    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        regfree(&regex);
        return -1; // Return -1 to indicate regex compilation failure
    }

    // Execute the regular expression
    reti = regexec(&regex, ipAddress, 0, NULL, 0);
    if (reti == 0)
    {
        printf("Valid IP Address: %s\n\n", ipAddress);
        regfree(&regex);
        clearInputBuffer();
        return 1; // Return 1 to indicate a valid IP address
    }
    else if (reti == REG_NOMATCH)
    {
        printf("Invalid IP Address: %s\n", ipAddress);
        regfree(&regex);
        return 0; // Return 0 to indicate an invalid IP address
    }
    else
    {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        regfree(&regex);
        return -1; // Return -1 to indicate regex execution failure
    }
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

int validateMask(char *mask)
{
    regex_t regex;
    int reti;

    // Compile the regular expression
    reti = regcomp(&regex, "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$", REG_EXTENDED);
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

void getAndVerifyIPAddress()
{
    char ip_address[16]; // Adjust the size according to your needs
    char mask[16]; // Adjust the size according to your needs
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