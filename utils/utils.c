#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include "utils.h"
#include "../database/database_functions.h"

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

void maskToBinary(char *subnetMask)
{
    char binaryString[33];
    // Convert dotted-decimal to binary
    unsigned long binaryMask = 0;
    int i = 24;
    char *octetStr;
    octetStr = strtok(subnetMask, ".");
    while (octetStr != NULL)
    {
        int octetValue = atoi(octetStr);
        if (octetValue < 0 || octetValue > 255)
        {
            free(subnetMask);
        }
        binaryMask |= (unsigned long)octetValue << i;
        i -= 8;
        octetStr = strtok(NULL, ".");
    }

    for (int j = 0; j < 32; j++)
    {
        binaryString[j] = ((binaryMask >> (31 - j)) & 1) ? '1' : '0';
    }

    binaryString[32] = '\0'; // Null-terminate the string
    printf("Binary representation: %s\n", binaryString);
}
