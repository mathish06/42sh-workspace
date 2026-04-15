/*
** EPITECH PROJECT, 2026
** utils.c
** File description:
** utils for the minishell1
*/
#include "../../include/my.h"

char *my_getenv(env_t *env, char *name)
{
    env_t *curr = env;

    while (curr != NULL) {
        if (my_strcmp(curr->name, name) == 0)
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}

void free_command_list(command_t *head)
{
    command_t *tmp;

    while (head != NULL) {
        tmp = head->next;
        free_tab(head->args);
        free(head->in_file);
        free(head->out_file);
        free(head);
        head = tmp;
    }
}

static int handle_backspace(char *buffer, int *i, int *max_len, char c)
{
    if (c != 127)
        return 0;
    if (*i > 0) {
        (*i)--;
        for (int pos = (*i); pos < (*max_len); pos++)
            buffer[pos] = buffer[pos + 1];
        (*max_len)--;
        write(1, "\b", 1);
        for (int tmp = (*i); tmp < (*max_len); tmp++)
            write(1, &buffer[tmp], 1);
        write(1, " ", 1);
        for (int tmp = (*max_len) + 1; tmp > (*i); tmp--)
            write(1, "\b", 1);
    }
    return 1;
}

static void insert_char(char *buffer, int *i, int *max_len, char c)
{
    int pos = (*max_len);
    int tmp = 0;

    while (pos > (*i)) {
        buffer[pos] = buffer[pos - 1];
        pos--;
    }
    buffer[*i] = c;
    (*max_len)++;
    (*i)++;
    for (tmp = (*i) - 1; tmp < (*max_len); tmp++)
        write(1, &buffer[tmp], 1);
    while (tmp > (*i)) {
        write(1, "\b", 1);
        tmp--;
    }
}

static int handle_regular_char(char *buffer, int *i, int *max_len, char c)
{
    if (c == '\n' || c == '\r') {
        write(1, "\n", 1);
        buffer[*i] = '\0';
        return 1;
    }
    if ((*i) < (*max_len)) {
        insert_char(buffer, i, max_len, c);
        return 0;
    }
    buffer[*i] = c;
    (*i)++;
    if (*i > *max_len)
        *max_len = *i;
    write(1, &c, 1);
    return 0;
}

static int handle_arrows(int *i, int total_length, char c)
{
    if (c != 27)
        return 0;
    read(0, &c, 1);
    if (c != '[')
        return 1;
    read(0, &c, 1);
    if (c == 'D' && *i > 0) {
        (*i)--;
        write(1, "\033[1D", 4);
    }
    if (c == 'C' && *i < total_length) {
        (*i)++;
        write(1, "\033[1C", 4);
    }
    return 1;
}

char *my_getline(void)
{
    char *buffer = malloc(sizeof(char) * 1024);
    int i = 0;
    char c;
    int txt = 0;
    int max_len = 0;

    if (buffer == NULL)
        return NULL;
    while (1) {
        txt = read(0, &c, 1);
        if (txt <= 0 || c == 4) {
            free(buffer);
            return NULL;
        }
        if (handle_backspace(buffer, &i, &max_len, c) == 1
            || handle_arrows(&i, max_len, c) == 1)
            continue;
        if (handle_regular_char(buffer, &i, &max_len, c) == 1)
            return buffer;
    }
}

char **tokens_to_array(token_t *head)
{
    int count = 0;
    char **args;
    int i = 0;
    token_t *curr = head;

    for (; curr != NULL; curr = curr->next)
        count++;
    args = malloc(sizeof(char *) * (count + 1));
    if (args == NULL)
        return NULL;
    curr = head;
    for (; curr != NULL; curr = curr->next) {
        args[i] = my_strdup(curr->value);
        i++;
    }
    args[i] = NULL;
    return args;
}
