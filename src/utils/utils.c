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

static int handle_backspace(int *i, char c)
{
    if (c != 127)
        return 0;
    if (*i > 0) {
        (*i)--;
        write(1, "\b \b", 3);
    }
    return 1;
}

static int handle_regular_char(char *buffer, int *i, char c)
{
    if (c == '\n' || c == '\r') {
        write(1, "\n", 1);
        buffer[*i] = '\0';
        return 1;
    }
    buffer[*i] = c;
    (*i)++;
    write(1, &c, 1);
    return 0;
}

char *my_getline(void)
{
    char *buffer = malloc(sizeof(char) * 1024);
    int i = 0;
    char c;
    int txt = 0;

    if (buffer == NULL)
        return NULL;
    while (1) {
        txt = read(0, &c, 1);
        if (txt <= 0 || c == 4) {
            free(buffer);
            return NULL;
        }
        if (handle_backspace(&i, c) == 1)
            continue;
        if (handle_regular_char(buffer, &i, c) == 1)
            return buffer;
    }
}
