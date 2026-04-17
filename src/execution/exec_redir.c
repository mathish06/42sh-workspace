/*
** EPITECH PROJECT, 2026
** exec_redir.c
** File description:
** exec_redir for 42sh
*/
#include "../../include/my.h"

static int handle_redir_right(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror(file);
        return 1;
    }
    dup2(fd, 1);
    close(fd);
    return 0;
}

static int handle_redir_double_right(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd == -1) {
        perror(file);
        return 1;
    }
    dup2(fd, 1);
    close(fd);
    return 0;
}

static int handle_redir_left(char *file)
{
    int fd = open(file, O_RDONLY);

    if (fd == -1) {
        perror(file);
        return 1;
    }
    dup2(fd, 0);
    close(fd);
    return 0;
}

static void fill_haredoc(char *end_world, int write_fd)
{
    char *line = NULL;

    while (1) {
        if (isatty(0))
            my_putstr("? ");
        line = my_getline();
        if (line == NULL)
            break;
        if (my_strcmp(line, end_world) == 0) {
            free(line);
            break;
        }
        write(write_fd, line, my_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
}

static int handle_redir_double_left(char *end_world)
{
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    fill_haredoc(end_world, pipefd[1]);
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
    return 0;
}

void exec_redir_node(ast_node_t *node, char **env, env_t **env_list)
{
    pid_t pid;
    int status;
    char *file = node->right->args[0];

    pid = fork();
    if (pid == 0) {
        if (node->type == NODE_REDIR_R && handle_redir_right(file) == 1)
            exit(1);
        if (node->type == NODE_REDIR_RR &&
            handle_redir_double_right(file) == 1)
            exit(1);
        if (node->type == NODE_REDIR_L && handle_redir_left(file) == 1)
            exit(1);
        if (node->type == NODE_REDIR_LL && handle_redir_double_left(file) == 1)
            exit(1);
        exec_ast(node->left, env, env_list);
        exit(0);
    }
    waitpid(pid, &status, 0);
}
