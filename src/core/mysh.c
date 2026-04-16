/*
** EPITECH PROJECT, 2026
** mysh.c
** File description:
** mysh for the minishell1
*/
#define _GNU_SOURCE
#include "../../include/my.h"

static char *build_history_path(env_t *env)
{
    char *home;
    char *path;
    int len;

    home = my_getenv(env, "HOME");
    if (home == NULL)
        return NULL;
    len = my_strlen(home) + my_strlen("/.42sh_history") + 1;
    path = malloc(len);
    if (path == NULL)
        return NULL;
    my_strcpy(path, home);
    my_strcat(path, "/.42sh_history");
    return path;
}

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

static void mysh_loop(mysh_t *shell, char **env)
{
    char *line = NULL;

    while (1) {
        if (isatty(0))
            my_putstr("$> ");
        line = my_getline(shell->history);
        if (handle_input(line, shell, env) == 1)
            break;
        free(line);
    }
    if (line != NULL)
        free(line);
}

int mysh(char **env)
{
    mysh_t shell;
    char *histpath;

    shell.env = env_to_list(env);
    shell.history = history_init(500);
    histpath = build_history_path(shell.env);
    history_load(shell.history, histpath);
    enable_raw_mode(&shell.original_term);
    mysh_loop(&shell, env);
    disable_raw_mode(&shell.original_term);
    history_save(shell.history, histpath);
    free(histpath);
    history_free(shell.history);
    free_env_list(shell.env);
    return 0;
}
