#!/bin/bash

# Compilation
gcc -o program main.c database/database_functions.c utils/utils.c validations/validations.c -lsqlite3

# Exécution
./program

# Suppression de l'exécutable
rm program