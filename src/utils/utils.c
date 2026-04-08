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
