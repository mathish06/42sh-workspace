/*
** EPITECH PROJECT, 2026
** exec_pipe.c
** File description:
** exec_pipe for 42sh
*/
#include "../../include/my.h"

static pid_t run_left_child(ast_node_t *node, char **env,
    env_t **env_list, int fd[2])
{
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        exec_ast(node->left, env, env_list);
        exit(0);
    }
    return pid;
}

static pid_t run_right_child(ast_node_t *node, char **env,
    env_t **env_list, int fd[2])
{
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        exec_ast(node->right, env, env_list);
        exit(0);
    }
    return pid;
}

void exec_pipe_node(ast_node_t *node, char **env, env_t **env_list)
{
    int fd[2];
    pid_t pid_left;
    pid_t pid_right;
    int status;

    if (pipe(fd) == -1)
        return;
    pid_left = run_left_child(node, env, env_list, fd);
    pid_right = run_right_child(node, env, env_list, fd);
    close(fd[0]);
    close(fd[1]);
    waitpid(pid_left, &status, 0);
    waitpid(pid_right, &status, 0);
}
