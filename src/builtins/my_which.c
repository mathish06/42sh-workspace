/*
** EPITECH PROJECT, 2025
** my_which.c
** File description:
** my_which for 42sh
*/
#include "../../include/my.h"

static int is_builtin(char *arg)
{
    const char *BUILTINS[] = {
        "cd", "env", "setenv", "unsetenv", "set", "unset", "alias", "unalias",
        "repeat", "which", "where", "exit", "echo", NULL
    };

    for (int i = 0; BUILTINS[i] != NULL; i++)
        if (my_strcmp(arg, BUILTINS[i]) == 0)
            return 1;
    return 0;
}

static int print_alias_or_builtin(alias_t *al, char *arg)
{
    if (al != NULL) {
        my_putstr(al->name);
        my_putstr(": \t aliased to ");
        my_putstr(al->value);
        my_putstr("\n");
        return 1;
    }
    if (is_builtin(arg) == 1) {
        my_putstr(arg);
        my_putstr(": shell built-in command.\n");
        return 1;
    }
    return 0;
}

static int analyze_one_arg(mysh_t *shell, char *arg)
{
    char *cmd;
    alias_t *al = find_alias(shell->alias, arg);

    if (print_alias_or_builtin(al, arg) == 1)
        return 0;
    cmd = find_command(arg, shell);
    if (cmd != NULL) {
        my_putstr(cmd);
        my_putstr("\n");
        free(cmd);
        return 0;
    }
    my_puterr(arg);
    my_puterr(": Command not found.\n");
    return 1;
}

int my_which(mysh_t *shell, char **args)
{
    int return_value = 0;

    if (args[1] == NULL) {
        my_puterr("which: Too few arguments.\n");
        return 1;
    }
    for (int i = 1; args[i] != NULL; i++) {
        if (analyze_one_arg(shell, args[i]) == 1)
            return_value = 1;
    }
    return return_value;
}
