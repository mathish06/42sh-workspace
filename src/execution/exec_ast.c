/*
** EPITECH PROJECT, 2026
** exec_ast.c
** File description:
** exec_ast for 42sh
*/
#include "../../include/my.h"

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
    if (node->type == NODE_REDIR_R || node->type == NODE_REDIR_RR ||
        node->type == NODE_REDIR_L || node->type == NODE_REDIR_LL)
        exec_redir_node(node, env, env_list);
}
