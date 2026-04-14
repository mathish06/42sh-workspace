/*
** EPITECH PROJECT, 2026
** mysh.c
** File description:
** mysh for the minishell1
*/
#define _GNU_SOURCE
#include "../../include/my.h"

static command_t *expand_and_build(char *line, mysh_t *shell)
{
    char *expanded;
    command_t *cmd_list;

    expanded = expand_history_events(line, shell->history);
    if (expanded == NULL)
        return NULL;
    if (my_strcmp(expanded, line) != 0) {
        my_putstr(expanded);
        my_putstr("\n");
    }
    history_add(shell->history, expanded);
    cmd_list = create_command_list(expanded);
    free(expanded);
    return cmd_list;
}

static int handle_input(char *line, mysh_t *shell, char **env)
{
    command_t *cmd_list;

    if (line == NULL) {
        if (isatty(0))
            my_putstr("exit\n");
        return 1;
    }
    if (my_strcmp(line, "exit") == 0) {
        my_putstr("exit\n");
        return 1;
    }
    if (line[0] != '\0') {
        cmd_list = expand_and_build(line, shell);
        if (cmd_list != NULL) {
            exec_command(cmd_list, env, &shell->env);
            free_command_list(cmd_list);
        }
    }
    return 0;
}

int mysh(char **env)
{
    char *line = NULL;
    mysh_t shell;

    shell.env = env_to_list(env);
    shell.history = history_init(500);
    enable_raw_mode(&shell.original_term);
    while (1) {
        if (isatty(0))
            my_putstr("$> ");
        line = my_getline(shell.history);
        if (handle_input(line, &shell, env) == 1)
            break;
        free(line);
    }
    disable_raw_mode(&shell.original_term);
    if (line != NULL)
        free(line);
    history_free(shell.history);
    free_env_list(shell.env);
    return 0;
}
