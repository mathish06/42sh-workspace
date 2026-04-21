/*
** EPITECH PROJECT, 2026
** manage_list.c
** File description:
** manage_list for the linked list
*/
#include "../../include/my.h"

env_t *create_node(char *line)
{
    env_t *node = malloc(sizeof(env_t));
    int i = 0;

    if (node == NULL)
        return NULL;
    while (line[i] != '=' && line[i] != '\0')
        i++;
    node->name = malloc(sizeof(char) * (i + 1));
    my_strncpy(node->name, line, i);
    node->name[i] = '\0';
    if (line[i] == '=')
        node->value = my_strdup(&line[i + 1]);
    else
        node->value = NULL;
    node->is_exported = 1;
    node->next = NULL;
    return node;
}

void add_node_end(env_t **head, char *line)
{
    env_t *node = create_node(line);
    env_t *tmp;

    if (node == NULL)
        return;
    if (*head == NULL) {
        *head = node;
        return;
    }
    tmp = *head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = node;
}

env_t *env_to_list(char **env)
{
    env_t *head = NULL;
    int i = 0;

    if (env == NULL)
        return NULL;
    while (env[i] != NULL) {
        add_node_end(&head, env[i]);
        i++;
    }
    return head;
}

void free_env_list(env_t *head)
{
    env_t *tmp;

    while (head != NULL) {
        tmp = head->next;
        if (head->name != NULL)
            free(head->name);
        if (head->value != NULL)
            free(head->value);
        free(head);
        head = tmp;
    }
}

static void remove_node(env_t **head, env_t *prev, env_t *curr)
{
    if (prev == NULL)
        *head = curr->next;
    else
        prev->next = curr->next;
    free(curr->name);
    if (curr->value != NULL)
        free(curr->value);
    free(curr);
}

void delete_env_node(env_t **head, char *name)
{
    env_t *curr = *head;
    env_t *prev = NULL;

    while (curr != NULL) {
        if (my_strcmp(curr->name, name) == 0) {
            remove_node(head, prev, curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
