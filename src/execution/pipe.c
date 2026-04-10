/*
** EPITECH PROJECT, 2026
** pipe.c
** File description:
** pipe for the minishell2
*/
#include "../../include/my.h"

static int count_pipe_chain(command_t *cmd_a)
{
    command_t *curr = cmd_a;
    int count = 1;

    while (curr != NULL && curr->separator == SEP_PIPE
        && curr->next != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

static void exec_pipe_child(command_t *cmd, pipe_ctx_t *ctx)
{
    char *cmd_path;

    manage_redirections(cmd);
    cmd_path = find_command(cmd->args[0], ctx->env_list);
    if (cmd_path == NULL) {
        write(2, cmd->args[0], my_strlen(cmd->args[0]));
        write(2, ": Command not found.\n", 21);
        exit(1);
    }
    execve(cmd_path, cmd->args, ctx->env);
    print_exec_error(cmd->args[0]);
    exit(1);
}

static pid_t spawn_pipe_child(command_t *cmd, pipe_ctx_t *ctx,
    int in_fd, int *pipefd)
{
    pid_t pid = fork();

    if (pid == -1)
        return -1;
    if (pid == 0) {
        if (in_fd != 0) {
            dup2(in_fd, 0);
            close(in_fd);
        }
        if (pipefd != NULL) {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        }
        exec_pipe_child(cmd, ctx);
    }
    return pid;
}

static int run_pipe_loop(command_t *cmd_a, pipe_ctx_t *ctx,
    int count, pid_t *pids)
{
    command_t *curr = cmd_a;
    int in_fd = 0;
    int pipefd[2];
    int i = 0;

    while (i < count) {
        if (i < count - 1 && pipe(pipefd) == -1)
            return 1;
        pids[i] = spawn_pipe_child(curr, ctx, in_fd,
            (i < count - 1) ? pipefd : NULL);
        if (in_fd != 0)
            close(in_fd);
        if (i < count - 1) {
            close(pipefd[1]);
            in_fd = pipefd[0];
        }
        curr = curr->next;
        i++;
    }
    return 0;
}

void exec_pipe(command_t *cmd_a, command_t *cmd_b, char **env, env_t *env_list)
{
    pipe_ctx_t ctx;
    int count = count_pipe_chain(cmd_a);
    pid_t *pids = malloc(sizeof(pid_t) * count);
    int status = 0;
    int i = 0;

    (void)cmd_b;
    ctx.env = env;
    ctx.env_list = env_list;
    if (pids == NULL)
        return;
    if (run_pipe_loop(cmd_a, &ctx, count, pids) != 0) {
        free(pids);
        return;
    }
    while (i < count) {
        waitpid(pids[i], &status, 0);
        i++;
    }
    free(pids);
}

command_t *run_pipe_chain(command_t *curr, char **env, env_t **env_list)
{
    exec_pipe(curr, curr->next, env, *env_list);
    while (curr != NULL && curr->separator == SEP_PIPE)
        curr = curr->next;
    if (curr == NULL)
        return NULL;
    return curr->next;
}
