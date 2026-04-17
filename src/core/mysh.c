/*
** EPITECH PROJECT, 2026
** mysh.c
** File description:
** mysh for the minishell1
*/
#define _GNU_SOURCE
#include "../../include/my.h"

void free_ast(ast_node_t *node)
{
    if (node == NULL)
        return;
    free_ast(node->left);
    free_ast(node->right);
    free_tab(node->args);
    free(node);
}

void free_tokens(token_t *head)
{
    token_t *curr = head;
    token_t *next_node;

    while (curr != NULL) {
        next_node = curr->next;
        if (curr->type == TOKEN_WORD && curr->value != NULL)
            free(curr->value);
        free(curr);
        curr = next_node;
    }
}

static int handle_input(char *line, char **env, env_t **env_list)
{
    token_t *tokens;
    ast_node_t *ast;

    if (line == NULL || my_strcmp(line, "exit") == 0) {
        my_putstr("exit\n");
        return 1;
    }
    if (line[0] == '\0')
        return 0;
    tokens = lexer(line);
    if (tokens == NULL)
        return 0;
    ast = build_ast(tokens);
    if (ast != NULL) {
        exec_ast(ast, env, env_list);
        free_ast(ast);
    }
    free_tokens(tokens);
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
