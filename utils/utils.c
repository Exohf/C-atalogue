#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include "../database/database_functions.h"
#include "../validations/validations.h"

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

char *toBinary(char *address)
{
    char *binaryString = (char *)malloc(33); // Allocate memory for the binary string
    if (binaryString == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Convert dotted-decimal to binary
    unsigned long binaryMask = 0;
    int i = 24;
    char *addressCopy = strdup(address); // Make a copy to avoid modifying the input string
    char *octetStr = strtok(addressCopy, ".");

    while (octetStr != NULL)
    {
        int octetValue = atoi(octetStr);
        if (octetValue < 0 || octetValue > 255)
        {
            free(addressCopy);
            free(binaryString);
            return NULL; // Indicate an error
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
    free(addressCopy);
    return binaryString;
}

int isNumeric(const char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            return 0; // Not a numeric string
        }
    }
    return 1; // Numeric string
}

void deleteEntryMenu(const char *db_name)
{
    int idToDelete;

    while (1)
    {
        // Ask the user for input
        printf("Enter the ID to delete (enter 'q' to go back to the menu): ");
        char userInput[20];
        if (fgets(userInput, sizeof(userInput), stdin) == NULL)
        {
            fprintf(stderr, "Error reading user input\n");
            return;
        }

        // Remove newline character from input
        userInput[strcspn(userInput, "\n")] = '\0';

        // Check if the user wants to go back to the menu
        if (strcmp(userInput, "q") == 0 || strcmp(userInput, "Q") == 0)
        {
            return;
        }

        // Validate the input as an integer
        if (!isNumeric(userInput))
        {
            printf("Invalid input. Please enter a valid numeric ID.\n");
            continue;
        }

        // Convert the input to an integer
        idToDelete = atoi(userInput);

        // Check if the ID is non-negative
        if (idToDelete < 0)
        {
            printf("Invalid input. Please enter a non-negative ID.\n");
            continue;
        }

        // Perform the deletion
        deleteEntryByID(db_name, idToDelete);
        break;
    }
}

int compareBinaryRepresentation(const char *binary)
{
    for (int i = 0; i < strlen(binary); i++)
    {
        printf("%c", binary[i]);
    }
}

void getMatchingIPsBySubnetMask(const char *db_name, char *ip_address_bin, int matchBits)
{
    sqlite3 *db = openDatabaseConnection(db_name);

    if (!db)
    {
        fprintf(stderr, "Failed to open the database\n");
        return;
    }

    char *sql = "SELECT * FROM IP_ADDRESS_LIST;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to execute the query: %s\n", sqlite3_errmsg(db));
        closeDatabaseConnection();
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        char *ipAddressText = (char *)sqlite3_column_text(stmt, 1);
        char *binaryRepresentation = toBinary(ipAddressText);

        if (strncmp(binaryRepresentation, ip_address_bin, matchBits) == 0)
        {
            printf("%s\n", ipAddressText);
        }

        free(binaryRepresentation);
    }

    sqlite3_finalize(stmt);
    closeDatabaseConnection();
}

void searchFromSameNetworkIp(const char *db_name)
{

    char ip_address[16];
    char mask[16];
    int valid = 0;
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

        if (verifyAddress(ip_address, 1) && getAndVerifyIpMask(mask))
        {
            valid = 1;
        }
    }

    int zeroCount = 0;
    char *ip_address_bin = toBinary(ip_address);
    char *mask_to_binary = toBinary(mask);

    for (int i = 0; i < 32; i++)
    {
        printf("%c", mask_to_binary[i]);
        if (mask_to_binary[i] == '0')
        {
            zeroCount++;
        }
    }

    int subnetMask = 32 - zeroCount;

    getMatchingIPsBySubnetMask(db_name, ip_address_bin, subnetMask);

    free(ip_address_bin);
    free(mask_to_binary);
}