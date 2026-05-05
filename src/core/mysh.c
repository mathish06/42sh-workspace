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
        if (curr->value != NULL)
            free(curr->value);
        free(curr);
        curr = next_node;
    }
}

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

static char *prepare_line(char *line, mysh_t *shell)
{
    char *expanded;
    char *with_backticks;

    expanded = expand_history_events(line, shell->history);
    if (expanded == NULL)
        return NULL;
    if (my_strcmp(expanded, line) != 0) {
        my_putstr(expanded);
        my_putstr("\n");
    }
    history_add(shell->history, expanded);
    if (quotes_check(expanded) == 0) {
        free(expanded);
        return NULL;
    }
    with_backticks = expand_backticks(expanded, shell);
    free(expanded);
    return with_backticks;
}

static int process_ast(char *expanded, mysh_t *shell, char **env)
{
    token_t *tokens;
    ast_node_t *ast;

    if (quotes_check(expanded) == 0)
        return 0;
    tokens = lexer(expanded);
    if (tokens == NULL)
        return 0;
    ast = build_ast(tokens);
    if (ast != NULL) {
        exec_ast(ast, env, shell);
        free_ast(ast);
    }
    return 0;
}

static int handle_input(char *line, mysh_t *shell, char **env)
{
    char *expanded;

    if (line == NULL || my_strcmp(line, "exit") == 0) {
        if (isatty(0))
            my_putstr("exit\n");
        return 1;
    }
    if (line[0] == '\0')
        return 0;
    expanded = prepare_line(line, shell);
    if (expanded != NULL) {
        process_ast(expanded, shell, env);
        free(expanded);
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
        if (handle_input(line, shell, env) == 1) {
            free(line);
            break;
        }
        free(line);
    }
}

int mysh(char **env)
{
    mysh_t shell;
    char *histpath;

    shell.alias = NULL;
    shell.last_status = 0;
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
