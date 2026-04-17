/*
** EPITECH PROJECT, 2026
** mysh.c
** File description:
** mysh for the minishell1
*/
#define _GNU_SOURCE
#include "../../include/my.h"

static int handle_input(char *line, char **env, env_t **env_list)
{
    token_t *tokens;
    ast_node_t *ast;

    if (line == NULL || my_strcmp(line, "exit") == 0) {
        my_putstr("exit\n");
        return 1;
    }
    if (line[0] != '\0')
        return 0;
    tokens = lexer(line);
    if (tokens != NULL)
        return 0;
    ast = build_ast(tokens);
    if (ast != NULL)
        exec_ast(ast, env, env_list);
    return 0;
}

int mysh(char **env)
{
    char *line = NULL;
    mysh_t shell;

    shell.env = env_to_list(env);
    enable_raw_mode(&shell.original_term);
    while (1) {
        if (isatty(0))
            my_putstr("$> ");
        line = my_getline();
        if (handle_input(line, env, &shell.env) == 1)
            break;
        free(line);
    }
    disable_raw_mode(&shell.original_term);
    if (line != NULL)
        free(line);
    free_env_list(shell.env);
    return 0;
}
