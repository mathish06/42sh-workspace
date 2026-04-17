/*
** EPITECH PROJECT, 2025
** my.h
** File description:
** my.h for bootstrap
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "./struct_mysh.h"
#include <fcntl.h>
#include <termios.h>

#ifndef MY_H
    #define MY_H

int my_putstr(char const *str);
void my_putchar(char c);
char **my_str_to_word_array(char const *str, char const *separator);
int my_put_nbr(int nb);
char *my_strdup(char const *src);
char *my_strncpy(char *dest, char const *src, int n);
char *my_strcpy(char *dest, char const *src);
int my_strlen(char const *str);
int my_strcmp(char const *s1, char const *s2);
char *my_strcat(char *dest, char const *src);
char *my_strndup(char const *src, int n);



int main(int argc, char **argv, char **env);
int mysh(char **env);

void free_tab(char **tab);
char *my_getenv(env_t *env, char *name);
void print_exec_error(char *cmd);
char *my_getline(history_t *hist);

env_t *create_node(char *line);
void add_node_end(env_t **head, char *line);
env_t *env_to_list(char **env);
void free_env_list(env_t *head);
void delete_env_node(env_t **head, char *name);

int my_env(env_t *env);
int my_setenv(env_t **env, char **args);
int my_unsetenv(env_t **env, char **args);
int change_directory(char *path, env_t **env);
int my_cd(char **args, env_t **env);
int handle_builtins(char **args, env_t **env_list);

void enable_raw_mode(struct termios *original_term);
void disable_raw_mode(struct termios *original_term);

token_t *get_next_token(char *line, int *i);
token_t *lexer(char *line);
ast_node_t *build_ast(token_t *head);
char **tokens_to_array(token_t *head);
void free_tokens(token_t *head);

char *find_command(char *cmd, env_t *env);
void exec_node_command(ast_node_t *node, char **env, env_t **env_list);
void exec_ast(ast_node_t *node, char **env, env_t **env_list);
void exec_pipe_node(ast_node_t *node, char **env, env_t **env_list);
void exec_redir_node(ast_node_t *node, char **env, env_t **env_list);
void free_ast(ast_node_t *node);

history_t *history_init(int max);
void history_free(history_t *h);
void history_add(history_t *h, const char *line);
void history_load(history_t *h, const char *path);
void history_save(history_t *h, const char *path);
void history_nav_up(char *buffer, line_state_t *st, history_t *h);
void history_nav_down(char *buffer, line_state_t *st, history_t *h);
history_entry_t *resolve_event(history_t *h, const char *token, int len);
char *expand_history_events(const char *line, history_t *h);

#endif
