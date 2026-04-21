/*
** EPITECH PROJECT, 2026
** alias_tools.c
** File description:
** alias_tools for the 42sh
*/
#include "../../include/my.h"

void free_alias_list(alias_t *head)
{
    alias_t *curr = head;
    alias_t *next_node;

    while (curr != NULL) {
        next_node = curr->next;
        if (curr->value != NULL)
            free(curr->value);
        if (curr->name != NULL)
            free(curr->name);
        free(curr);
        curr = next_node;
    }
}

alias_t *find_alias(alias_t *head, char *alias_name)
{
    alias_t *curr = head;

    while (curr != NULL) {
        if (my_strcmp(curr->name, alias_name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}
