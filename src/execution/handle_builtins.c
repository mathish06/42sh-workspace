/*
** EPITECH PROJECT, 2026
** handle_builtins.c
** File description:
** handle_builtins for 42sh
*/
#include "../../include/my.h"

int handle_builtins(char **args, env_t **env_list)
{
    if (my_strcmp(args[0], "env") == 0) {
        my_env(*env_list);
        return 1;
    }
    if (my_strcmp(args[0], "setenv") == 0) {
        my_setenv(env_list, args);
        return 1;
    }
    if (my_strcmp(args[0], "unsetenv") == 0) {
        my_unsetenv(env_list, args);
        return 1;
    }
    if (my_strcmp(args[0], "cd") == 0) {
        my_cd(args, env_list);
        return 1;
    }
    if (my_strcmp(args[0], "set") == 0) {
        my_set(env_list, args);
        return 1;
    }
    if (my_strcmp(args[0], "unset") == 0) {
        my_unset(env_list, args);
        return 1;
    }
    return 0;
}
