/*
** EPITECH PROJECT, 2026
** mysh.c
** File description:
** mysh for the minishell1
*/
#define _GNU_SOURCE
#include "../include/my.h"

static int handle_input(char *line, ssize_t read, char **env, env_t **env_list)
{
    command_t *cmd_list;

    if (read == -1) {
        my_putstr("exit\n");
        return 1;
    }
    if (line[read - 1] == '\n')
        line[read - 1] = '\0';
    if (my_strcmp(line, "exit") == 0) {
        my_putstr("exit\n");
        return 1;
    }
    if (line[0] != '\0') {
        cmd_list = create_command_list(line);
        if (cmd_list != NULL) {
            exec_command(cmd_list, env, env_list);
            free_command_list(cmd_list);
        }
    }
    return 0;
}

int mysh(char **env)
{
    env_t *env_list = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    env_list = env_to_list(env);
    while (1) {
        if (isatty(0))
            my_putstr("$> ");
        read = getline(&line, &len, stdin);
        if (handle_input(line, read, env, &env_list) == 1)
            break;
    }
    if (line != NULL)
        free(line);
    free_env_list(env_list);
    return 0;
}
