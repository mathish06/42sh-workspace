/*
** EPITECH PROJECT, 2026
** handle_builtins.c
** File description:
** handle_builtins for 42sh
*/

#include "../../include/my.h"

static int handle_local_vars(char **args, mysh_t *shell)
{
    if (my_strcmp(args[0], "set") == 0) {
        my_set(shell, args);
        return 1;
    }
    if (my_strcmp(args[0], "unset") == 0) {
        my_unset(shell, args);
        return 1;
    }
    if (my_strcmp(args[0], "alias") == 0) {
        my_alias(shell, args);
        return 1;
    }
    if (my_strcmp(args[0], "unalias") == 0) {
        my_unalias(shell, args);
        return 1;
    }
    return 0;
}

static int handle_local_vars_second(char **args, mysh_t *shell)
{
    if (my_strcmp(args[0], "repeat") == 0) {
        my_repeat(shell, args);
        return 1;
    }
    return 0;
}

int handle_builtins(char **args, mysh_t *shell)
{
    if (my_strcmp(args[0], "env") == 0) {
        my_env(shell);
        return 1;
    }
    if (my_strcmp(args[0], "setenv") == 0) {
        my_setenv(shell, args);
        return 1;
    }
    if (my_strcmp(args[0], "unsetenv") == 0) {
        my_unsetenv(shell, args);
        return 1;
    }
    if (my_strcmp(args[0], "cd") == 0) {
        my_cd(args, shell);
        return 1;
    }
    if (handle_local_vars(args, shell) == 1)
        return 1;
    return handle_local_vars_second(args, shell);
}
