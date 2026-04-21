/*
** EPITECH PROJECT, 2025
** my_alias.c
** File description:
** my_alias for the 42sh
*/
#include "../../include/my.h"

static int one_arg(mysh_t *shell)
{
    alias_t *curr = shell->alias;

    while (curr != NULL) {
        my_putstr(curr->name);
        my_putchar('\t');
        my_putstr(curr->value);
        my_putchar('\n');
        curr = curr->next;
    }
    return 0;
}

static int two_args(mysh_t *shell, char **args)
{
    alias_t *target = NULL;

    target = find_alias(shell->alias, args[1]);
    if (target != NULL) {
        my_putstr(target->value);
        my_putchar('\n');
        return 0;
    }
    return 0;
}

static int three_args(mysh_t *shell, char **args)
{
    int len = 0;
    char *full_value;

    for (int i = 2; args[i] != NULL; i++) {
        len += my_strlen(args[i]);
        len++;
    }
    full_value = malloc(sizeof(char) * len);
    if (full_value == NULL)
        return 84;
    full_value = my_strcpy(full_value, args[2]);
    for (int i = 3; args[i] != NULL; i++) {
        full_value = my_strcat(full_value, " ");
        full_value = my_strcat(full_value, args[i]);
    }
    add_alias(&(shell->alias), args[1], full_value);
    free(full_value);
    return 0;
}

int my_alias(mysh_t *shell, char **args)
{
    if (args[1] == NULL)
        return one_arg(shell);
    if (args[2] == NULL)
        return two_args(shell, args);
    return three_args(shell, args);
}
