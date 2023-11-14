#include <stdio.h>
#include "utils/utils.h"
#include "database/database_functions.h"
#include "validations/validations.h"

#define DB_NAME "IP_ADDRESS_LIST.db"

void start()
{
    clearConsole();

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
        getAndVerifyIPAddress(DB_NAME);
        break;
    case 'c':
        clearConsole();
        break;
    case 'd':
        deleteEntryMenu(DB_NAME);
        break;
    case 'h':
        displayHelper();
        break;
    case 'l':
        listAllEntries(DB_NAME);
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