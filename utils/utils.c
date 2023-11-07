#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "database_functions.h"

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
