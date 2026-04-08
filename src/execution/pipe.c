/*
** EPITECH PROJECT, 2026
** pipe.c
** File description:
** pipe for the minishell2
*/
#include "../../include/my.h"

static void exec_left_child(command_t *cmd_a, char **env, env_t *env_list,
    int fd[2])
{
    char *cmd_path;

    close(fd[0]);
    dup2(fd[1], 1);
    close(fd[1]);
    manage_redirections(cmd_a);
    cmd_path = find_command(cmd_a->args[0], env_list);
    if (cmd_path == NULL) {
        write(2, cmd_a->args[0], my_strlen(cmd_a->args[0]));
        write(2, ": Command not found.\n", 21);
        exit(1);
    }
    execve(cmd_path, cmd_a->args, env);
    perror(cmd_a->args[0]);
    exit(1);
}

static void exec_right_child(command_t *cmd_b, char **env, env_t *env_list,
    int fd[2])
{
    char *cmd_path;

    close(fd[1]);
    dup2(fd[0], 0);
    close(fd[0]);
    manage_redirections(cmd_b);
    cmd_path = find_command(cmd_b->args[0], env_list);
    if (cmd_path == NULL) {
        write(2, cmd_b->args[0], my_strlen(cmd_b->args[0]));
        write(2, ": Command not found.\n", 21);
        exit(1);
    }
    execve(cmd_path, cmd_b->args, env);
    perror(cmd_b->args[0]);
    exit(1);
}

void exec_pipe(command_t *cmd_a, command_t *cmd_b, char **env, env_t *env_list)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;
    int status;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }
    pid1 = fork();
    if (pid1 == 0)
        exec_left_child(cmd_a, env, env_list, fd);
    pid2 = fork();
    if (pid2 == 0)
        exec_right_child(cmd_b, env, env_list, fd);
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
}
