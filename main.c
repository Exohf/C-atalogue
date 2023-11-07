#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "database/database_functions.h"
#include "utils/utils.h"
// sudo apt-get install libsqlite3-dev
// gcc -o a.out main.c -lsqlite3
// #include <regex.h>
// REGEX IP ADRESS = (\b25[0-5]|\b2[0-4][0-9]|\b[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3} : https://ihateregex.io/expr/ip/
// Souhaitez-vous continuer ? [O/n]
#define MAX_IP_LENGTH 16
#define MAX_IP_COUNT 100
#define DB_NAME "IP_ADDRESS_LIST.db"

char ip_addresses[MAX_IP_COUNT][MAX_IP_LENGTH];
int ip_count = 0;

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

    warnHelpCommand();

    free(message_copy);
}

void handleInput(char command)
{
    switch (command)
    {
    case 'a':
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
        listTableContents(DB_NAME);
        break;
    case 'q':
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
        printf("Enter a command : ");
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