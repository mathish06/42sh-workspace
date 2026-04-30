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
        my_puterr("Segmentation fault");
    if (sig_no == SIGFPE)
        my_puterr("Floating exception");
    if (WCOREDUMP(status))
        my_puterr(" (core dumped)");
    write(2, "\n", 1);
}

static void run_child_process(char *path, ast_node_t *node, mysh_t *shell)
{
    pid_t pid;
    int status;
    char **env_array;

    pid = fork();
    if (pid == 0) {
        env_array = env_list_to_tab(shell->env);
        execve(path, node->args, env_array);
        print_exec_error(path);
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->last_status = WEXITSTATUS(status);
        if (WIFSIGNALED(status)) {
            check_program_status(status);
            shell->last_status = 1;
        }
    }
}

void exec_node_command(ast_node_t *node, char **env, mysh_t *shell)
{
    char *cmd_path = NULL;

    (void) env;
    if (node == NULL || node->args == NULL || node->args[0] == NULL)
        return;
    expand_aliases(node, shell);
    if (node->args == NULL || node->args[0] == NULL)
        return;
    if (handle_builtins(node->args, shell) == 1)
        return;
    cmd_path = find_command(node->args[0], (shell));
    if (cmd_path == NULL) {
        my_puterr(node->args[0]);
        my_puterr(": Command not found.\n");
        shell->last_status = 1;
        return;
    }
    run_child_process(cmd_path, node, shell);
    free(cmd_path);
}

static void exec_logic_nodes(ast_node_t *node, char **env, mysh_t *shell)
{
    if (node->type == NODE_SEPARATOR) {
        exec_ast(node->left, env, shell);
        exec_ast(node->right, env, shell);
    }
    if (node->type == NODE_AND) {
        exec_ast(node->left, env, shell);
        if (shell->last_status == 0)
            exec_ast(node->right, env, shell);
    }
    if (node->type == NODE_OR) {
        exec_ast(node->left, env, shell);
        if (shell->last_status != 0)
            exec_ast(node->right, env, shell);
    }
}

void exec_ast(ast_node_t *node, char **env, mysh_t *shell)
{
    if (node == NULL)
        return;
    if (node->type == NODE_COMMAND)
        exec_node_command(node, env, shell);
    if (node->type == NODE_SEPARATOR || node->type == NODE_AND ||
        node->type == NODE_OR)
        exec_logic_nodes(node, env, shell);
    if (node->type == NODE_PIPE)
        exec_pipe_node(node, env, shell);
    if (node->type == NODE_REDIR_R || node->type == NODE_REDIR_RR ||
        node->type == NODE_REDIR_L || node->type == NODE_REDIR_LL)
        exec_redir_node(node, env, shell);
}
