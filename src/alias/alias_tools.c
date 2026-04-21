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

void add_alias(alias_t **head ,char *alias_name, char *alias_value)
{
    alias_t *target = find_alias(*head, alias_name);
    alias_t *new_node;

    if (target != NULL) {
        free(target->value);
        target->value = my_strdup(alias_value);
        return;
    }
    new_node = malloc(sizeof(alias_t));
    if (new_node == NULL)
        return;
    new_node->name = my_strdup(alias_name);
    new_node->value = my_strdup(alias_value);
    new_node->next = *head;
    *head = new_node;
}
