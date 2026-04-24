/*
** EPITECH PROJECT, 2026
** alias_replacement.c
** File description:
** alias_replacement for 42 sh
*/
#include "../../include/my.h"

static int compt_tab(char **tab)
{
    int compt = 0;
    int i = 0;

    while (tab[i] != NULL) {
        compt++;
        i++;
    }
    return (compt);
}

char **replace_args_with_alias(char **old_args, char **alias_args)
{
    int old_size = compt_tab(old_args);
    int new_size = compt_tab(alias_args);
    char **full;
    int index = 0;

    full = malloc(sizeof(char *) * (old_size - 1 + new_size + 1));
    if (full == NULL)
        return NULL;
    for (int i = 0; alias_args[i] != NULL; i++) {
        full[index] = my_strdup(alias_args[i]);
        index++;
    }
    for (int i = 1; old_args[i] != NULL; i++) {
        full[index] = my_strdup(old_args[i]);
        index++;
    }
    full[index] = NULL;
    free_tab(old_args);
    free_tab(alias_args);
    return full;
}
