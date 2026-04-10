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
void exec_command(command_t *cmd_list, char **env, env_t **env_list);
void free_tab(char **tab);
env_t *create_node(char *line);
void add_node_end(env_t **head, char *line);
env_t *env_to_list(char **env);
int mysh(char **env);
char *my_getenv(env_t *env, char *name);
char *find_command(char *cmd, env_t *env);
void free_env_list(env_t *head);
int my_env(env_t *env);
int my_setenv(env_t **env, char **args);
void delete_env_node(env_t **head, char *name);
int my_unsetenv(env_t **env, char **args);
void print_exec_error(char *cmd);
int change_directory(char *path, env_t **env);
int my_cd(char **args, env_t **env);
command_t *create_command_list(char *line);
void manage_redirections(command_t *cmd);
void exec_pipe(command_t *cmd_a, command_t *cmd_b, char **env, env_t *env_list);
command_t *run_pipe_chain(command_t *curr, char **env, env_t **env_list);
void free_command_list(command_t *head);

#endif
