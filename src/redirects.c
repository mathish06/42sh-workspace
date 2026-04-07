/*
** EPITECH PROJECT, 2026
** redirects.c
** File description:
** redirects for the minishell2
*/
#include "../include/my.h"

static void handle_redir_right(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror(file);
        exit(1);
    }
    dup2(fd, 1);
    close(fd);
}

static void handle_redir_double_right(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd == -1) {
        perror(file);
        exit(1);
    }
    dup2(fd, 1);
    close(fd);
}

static void handle_redir_left(char *file)
{
    int fd = open(file, O_RDONLY);

    if (fd == -1) {
        perror(file);
        exit(1);
    }
    dup2(fd, 0);
    close(fd);
}

static void fill_haredoc(char *end_world, int write_fd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;

    while (1) {
        if (isatty(0))
            my_putstr("? ");
        read_bytes = getline(&line, &len, stdin);
        if (read_bytes == -1)
            break;
        if (line[read_bytes - 1] == '\n')
            line[read_bytes - 1] = '\0';
        if (my_strcmp(line, end_world) == 0)
            break;
        line[read_bytes - 1] = '\n';
        write(write_fd, line, read_bytes);
    }
    free(line);
}

static void handle_redir_double_left(char *end_world)
{
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    fill_haredoc(end_world, pipefd[1]);
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
}

void manage_redirections(command_t *cmd)
{
    if (cmd->out_type == REDIR_RIGHT)
        handle_redir_right(cmd->out_file);
    if (cmd->out_type == REDIR_DOUBLE_RIGHT)
        handle_redir_double_right(cmd->out_file);
    if (cmd->in_type == REDIR_LEFT)
        handle_redir_left(cmd->in_file);
    if (cmd->in_type == REDIR_DOUBLE_LEFT)
        handle_redir_double_left(cmd->in_file);
}
