#include <stdio.h>
#include "utils/utils.h"
#include "database/database_functions.h"

#define MAX_IP_LENGTH 16
#define MAX_IP_COUNT 100
#define DB_NAME "IP_ADDRESS_LIST.db"

char ip_addresses[MAX_IP_COUNT][MAX_IP_LENGTH];
int ip_count = 0;

void start()
{
    // clearConsole();

    const char *message = ":::::::                   ::: ::::::::::: :::     :::        ::::::::   ::::::::  :::    ::: ::::::::::\n"
                          ":+:    :+:                :+: :+:   :+:   :+: :+:   :+:       :+:    :+: :+:    :+: :+:    :+: :+:\n"
                          "+:+                      +:+   +:+  +:+  +:+   +:+  +:+       +:+    +:+ +:+        +:+    +:+ +:+\n"
                          "+#+       +#++:++#++:++ +#++:++#++: +#+ +#++:++#++: +#+       +#+    +:+ :#:        +#+    +:+ +#++:++#\n"
                          "+#+                     +#+     +#+ +#+ +#+     +#+ +#+       +#+    +#+ +#+   +#+# +#+    +#+ +#+\n"
                          "#+#    #+#              #+#     #+# #+# #+#     #+# #+#       #+#    #+# #+#    #+# #+#    #+# #+#\n"
                          "########               ###     ### ### ###     ### ########## ########   ########   ########  ##########\n";
    //(GPT)
    const char *delimiter = "\n";
    char *message_copy = strdup(message);
    char *line = strtok(message_copy, delimiter);

    while (line != NULL)
    {
        printf("%s\n", line);
        usleep(200000);
        line = strtok(NULL, delimiter);
    }

    initializeDatabase(DB_NAME);
    warnHelpCommand();

    free(message_copy);
}

void handleInput(char command)
{
    switch (command)
    {
    case 'a':
        getAndVerifyIPAddress();
        break;
    case 'c':
        clearConsole();
        break;
    case 'd':
        printf("\nCommand 'd' selected: Delete an IP.\n");
        break;
    case 'h':
        displayHelper();
        break;
    case 'l':
        break;
    case 'q':
        closeDatabaseConnection();
        printf("\nExiting the program...\n");
        exit(0);
    case 's':
        printf("\nCommand 's' selected: Search similar by mask.\n");
        break;
    default:
        handleInvalidCommand();
        break;
    }
}
// Souhaitez-vous continuer ? [O/n]
int main()
{
    start();

    while (1)
    {
        printf("Enter command : ");
        char command = getchar();

        // Fix null "press enter" issue
        if (command == 0x0a)
        {
            handleInvalidCommand();
            continue;
        }

        clearInputBuffer();

        handleInput(command);
    }

    return 0;
}