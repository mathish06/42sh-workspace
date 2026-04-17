/*
** EPITECH PROJECT, 2026
** utils_part_two.c
** File description:
** utils_part_two for 42sh
*/
#include "../../include/my.h"

void free_tab(char **tab)
{
    int i = 0;

    if (tab == NULL)
        return;
    while (tab[i] != NULL) {
        free(tab[i]);
        i++;
    }
    free(tab);
}

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
