/*
** EPITECH PROJECT, 2026
** my_unalias.c
** File description:
** my_unalias for 42sh
*/
#include "../../include/my.h"

int my_unalias(mysh_t *shell, char **args)
{
    if (args[1] == NULL) {
        write(2, "unalias: Too few arguments.\n", 28);
        return 1;
    }
    for (int i = 1; args[i] != NULL; i++)
        delete_alias(&(shell->alias), args[i]);
    return 0;
}
