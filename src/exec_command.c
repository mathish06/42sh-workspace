/*
** EPITECH PROJECT, 2026
** exec_command.c
** File description:
** exec_command for the minishell 1
*/
#include "../include/my.h"

void free_tab(char **tab)
{
    int i = 0;

    if (tab == NULL)
        return;
    while (tab[i] != NULL) {
        free(tab[i]);
        i++;
    }
    free(tab);
}

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

static void execute_child(char *cmd_path, char **args, char **env,
    command_t *cmd)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        manage_redirections(cmd);
        execve(cmd_path, args, env);
        print_exec_error(cmd_path);
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
            check_program_status(status);
    }
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

static void execute_binary(char **args, char **env, env_t *env_list,
    command_t *cmd)
{
    char *cmd_path;

    cmd_path = find_command(args[0], env_list);
    if (cmd_path == NULL) {
        my_putstr(args[0]);
        my_putstr(": Command not found.\n");
        return;
    }
    execute_child(cmd_path, args, env, cmd);
    free(cmd_path);
}

void exec_command(command_t *cmd_list, char **env, env_t **env_list)
{
    command_t *curr = cmd_list;

    while (curr != NULL) {
        if (curr->args == NULL || curr->args[0] == NULL) {
            curr = curr->next;
            continue;
        }
        if (curr->separator == SEP_PIPE && curr->next != NULL) {
            exec_pipe(curr, curr->next, env, *env_list);
            curr = curr->next->next;
            continue;
        }
        if (handle_builtins(curr->args, env_list) == 0)
            execute_binary(curr->args, env, *env_list, curr);
        curr = curr->next;
    }
}
