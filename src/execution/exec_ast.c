/*
** EPITECH PROJECT, 2026
** exec_ast.c
** File description:
** exec_ast for 42sh
*/
#include "../../include/my.h"

static int has_slash(char *str)
{
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '/')
            return 1;
        i++;
    }
    return 0;
}

static char *try_path_composition(char *dir, char *cmd)
{
    int len;
    char *full;

    len = my_strlen(dir) + my_strlen(cmd) + 2;
    full = malloc(sizeof(char) * len);
    if (full == NULL)
        return NULL;
    my_strcpy(full, dir);
    my_strcat(full, "/");
    my_strcat(full, cmd);
    if (access(full, X_OK) == 0)
        return full;
    free(full);
    return NULL;
}

static char *search_in_paths(char **path_tab, char *cmd)
{
    char *full_path;
    int i = 0;

    if (path_tab == NULL)
        return NULL;
    while (path_tab[i] != NULL) {
        full_path = try_path_composition(path_tab[i], cmd);
        if (full_path != NULL)
            return full_path;
        i++;
    }
    return NULL;
}

static void check_program_status(int status)
{
    int sig_no = WTERMSIG(status);

    if (sig_no == SIGSEGV)
        my_putstr("Segmentation fault");
    if (sig_no == SIGFPE)
        my_putstr("Floating exception");
    if (WCOREDUMP(status))
        my_putstr(" (core dumped)");
    my_putchar('\n');
}

static int handle_builtins(char **args, env_t **env_list)
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
    return 0;
}

char *find_command(char *cmd, env_t *env)
{
    char *result;
    char **path_tab;
    char *full_path;

    if (has_slash(cmd))
        return my_strdup(cmd);
    result = my_getenv(env, "PATH");
    if (result == NULL)
        return NULL;
    path_tab = my_str_to_word_array(result, ":");
    full_path = search_in_paths(path_tab, cmd);
    free_tab(path_tab);
    return full_path;
}

static void run_child_process(char *path, ast_node_t *node, char **env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        execve(path, node->args, env);
        print_exec_error(path);
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
            check_program_status(status);
    }
}

void exec_node_command(ast_node_t *node, char **env, env_t **env_list)
{
    char *cmd_path = NULL;

    if (node == NULL || node->args == NULL || node->args[0] == NULL)
        return;
    if (handle_builtins(node->args, env_list) == 1)
        return;
    cmd_path = find_command(node->args[0], *env_list);
    if (cmd_path == NULL) {
        my_putstr(node->args[0]);
        my_putstr(": Command not found.\n");
        return;
    }
    run_child_process(cmd_path, node, env);
    free(cmd_path);
}

void exec_ast(ast_node_t *node, char **env, env_t **env_list)
{
    if (node == NULL)
        return;
    if (node->type == NODE_COMMAND)
        exec_node_command(node, env, env_list);
    if (node->type == NODE_SEPARATOR) {
        exec_ast(node->left, env, env_list);
        exec_ast(node->right, env, env_list);
    }
    if (node->type == NODE_PIPE)
        exec_pipe_node(node, env, env_list);
}
