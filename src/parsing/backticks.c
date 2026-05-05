/*
** EPITECH PROJECT, 2026
** backticks.c
** File description:
** command substitution (backticks)
*/

#include "../../include/my.h"

static void append_to_res(char *result, char *buffer, int n)
{
    buffer[n] = '\0';
    my_strcat(result, buffer);
}

static char *read_pipe_output(int fd)
{
    char *result = malloc(sizeof(char) * 4096);
    char buffer[1024];
    int total = 0;
    int n;

    if (result == NULL)
        return NULL;
    result[0] = '\0';
    n = read(fd, buffer, 1023);
    while (n > 0 && total < 3000) {
        append_to_res(result, buffer, n);
        total += n;
        n = read(fd, buffer, 1023);
    }
    if (total > 0 && result[total - 1] == '\n')
        result[total - 1] = '\0';
    return result;
}

static void run_inner_child(char *cmd, mysh_t *shell, int *pipefd)
{
    token_t *tokens;
    ast_node_t *ast;

    dup2(pipefd[1], 1);
    close(pipefd[0]);
    close(pipefd[1]);
    tokens = lexer(cmd);
    ast = build_ast(tokens);
    if (ast != NULL) {
        exec_ast(ast, NULL, shell);
        free_ast(ast);
    }
    exit(0);
}

static char *exec_inner_command(char *cmd, mysh_t *shell)
{
    int pipefd[2];
    pid_t pid;
    char *output;

    if (pipe(pipefd) == -1)
        return my_strdup("");
    pid = fork();
    if (pid == 0)
        run_inner_child(cmd, shell, pipefd);
    close(pipefd[1]);
    waitpid(pid, NULL, 0);
    output = read_pipe_output(pipefd[0]);
    close(pipefd[0]);
    return output;
}

static int find_backtick_end(char *line, int start)
{
    int i = start + 1;

    while (line[i] != '\0') {
        if (line[i] == '`')
            return i;
        i++;
    }
    return -1;
}

static char *build_replacement(char *line, char *output, int start, int end)
{
    char *result;
    int new_len;

    new_len = start + my_strlen(output) + my_strlen(&line[end + 1]) + 1;
    result = malloc(sizeof(char) * new_len);
    if (result == NULL)
        return NULL;
    my_strncpy(result, line, start);
    result[start] = '\0';
    my_strcat(result, output);
    my_strcat(result, &line[end + 1]);
    return result;
}

static char *replace_backtick(char *line, int start, int end, mysh_t *shell)
{
    char *inner = my_strndup(&line[start + 1], end - start - 1);
    char *output = exec_inner_command(inner, shell);
    char *result = build_replacement(line, output, start, end);

    free(inner);
    free(output);
    return result;
}

static char *one_backtick(char *current, int i, mysh_t *shell)
{
    int end = find_backtick_end(current, i);
    char *new_line;

    if (end == -1)
        return current;
    free(current);
    return new_line;
}

char *expand_backticks(char *line, mysh_t *shell)
{
    char *current = my_strdup(line);
    int i = 0;

    if (current == NULL)
        return NULL;
    while (current != NULL && current[i] != '\0') {
        if (current[i] == '`') {
            current = one_backtick(current, i, shell);
            continue;
        }
        i++;
    }
    return current;
}
