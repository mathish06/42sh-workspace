/*
** EPITECH PROJECT, 2025
** parsing.c
** File description:
** parsing for the minishell2
*/
#include "../../include/my.h"

static char *extract_filename(char *raw_cmd, int *i)
{
    int start;
    int len;
    char *file;

    while (raw_cmd[*i] == ' ' || raw_cmd[*i] == '\t')
        (*i)++;
    start = *i;
    while (raw_cmd[*i] != '\0' && raw_cmd[*i] != ' ' &&
        raw_cmd[*i] != '\t' && raw_cmd[*i] != '<' && raw_cmd[*i] != '>')
        (*i)++;
    len = *i - start;
    file = malloc(sizeof(char) * (len + 1));
    if (file == NULL)
        return NULL;
    for (int k = 0; k < len; k++) {
        file[k] = raw_cmd[start + k];
        raw_cmd[start + k] = ' ';
    }
    file[len] = '\0';
    return file;
}

static void handle_right_redir(char *raw_cmd, command_t *cmd, int *i)
{
    if (raw_cmd[*i + 1] == '>') {
        cmd->out_type = REDIR_DOUBLE_RIGHT;
        raw_cmd[*i] = ' ';
        raw_cmd[*i + 1] = ' ';
        (*i) += 2;
    } else {
        cmd->out_type = REDIR_RIGHT;
        raw_cmd[*i] = ' ';
        (*i)++;
    }
    cmd->out_file = extract_filename(raw_cmd, i);
}

static void handle_left_redir(char *raw_cmd, command_t *cmd, int *i)
{
    if (raw_cmd[*i + 1] == '<') {
        cmd->in_type = REDIR_DOUBLE_LEFT;
        raw_cmd[*i] = ' ';
        raw_cmd[*i + 1] = ' ';
        (*i) += 2;
    } else {
        cmd->in_type = REDIR_LEFT;
        raw_cmd[*i] = ' ';
        (*i)++;
    }
    cmd->in_file = extract_filename(raw_cmd, i);
}

static void extract_redirections(char *raw_cmd, command_t *cmd)
{
    cmd->in_type = REDIR_NONE;
    cmd->out_type = REDIR_NONE;
    cmd->in_file = NULL;
    cmd->out_file = NULL;
    for (int i = 0; raw_cmd[i] != '\0'; i++) {
        if (raw_cmd[i] == '>')
            handle_right_redir(raw_cmd, cmd, &i);
        if (raw_cmd[i] == '<')
            handle_left_redir(raw_cmd, cmd, &i);
    }
}

static void add_cmd_to_list(command_t **head, command_t **tail, command_t *cmd)
{
    cmd->next = NULL;
    if (*head == NULL) {
        *head = cmd;
        *tail = cmd;
    } else {
        (*tail)->next = cmd;
        *tail = cmd;
    }
}

static separator_t get_separator(char **pipe_arr, char **semi_arr, int i, int j)
{
    if (pipe_arr[j + 1] != NULL)
        return SEP_PIPE;
    if (semi_arr[i + 1] != NULL)
        return SEP_SEMI;
    return SEP_NONE;
}

static void process_pipe_arr(char **pipe_arr, char **semi_arr, int i,
    command_t **head_tail)
{
    command_t *cmd;

    for (int j = 0; pipe_arr[j] != NULL; j++) {
        cmd = malloc(sizeof(command_t));
        if (cmd == NULL)
            return;
        extract_redirections(pipe_arr[j], cmd);
        cmd->args = my_str_to_word_array(pipe_arr[j], " \t");
        cmd->separator = get_separator(pipe_arr, semi_arr, i, j);
        add_cmd_to_list(&head_tail[0], &head_tail[1], cmd);
    }
}

command_t *create_command_list(char *line)
{
    command_t *head_tail[2] = {NULL, NULL};
    char **semi_arr;
    char **pipe_arr;

    semi_arr = my_str_to_word_array(line, ";");
    if (semi_arr == NULL)
        return NULL;
    for (int i = 0; semi_arr[i] != NULL; i++) {
        pipe_arr = my_str_to_word_array(semi_arr[i], "|");
        if (pipe_arr != NULL) {
            process_pipe_arr(pipe_arr, semi_arr, i, head_tail);
            free_tab(pipe_arr);
        }
    }
    free_tab(semi_arr);
    return head_tail[0];
}
