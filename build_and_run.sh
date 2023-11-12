#!/bin/bash

# Options par défaut
DEBUG=0

# Analyse des options en ligne de commande
while getopts ":b" opt; do
  case $opt in
    b)
      DEBUG=1
      ;;
    \?)
      echo "Option invalide: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

# Compilation avec ou sans options de débogage
if [ "$DEBUG" -eq 1 ]; then
  gcc -g -o program main.c database/database_functions.c utils/utils.c validations/validations.c -lsqlite3
else
  gcc -o program main.c database/database_functions.c utils/utils.c validations/validations.c -lsqlite3
fi

# Exécution
if [ "$DEBUG" -eq 1 ]; then
  gdb ./program
else
  ./program
fi

# Suppression de l'exécutable
rm program