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

static int count_exported(env_t *head)
{
    int count = 0;

    for (env_t *curr = head; curr != NULL; curr = curr->next) {
        if (curr->is_exported == 1)
            count++;
    }
    return count;
}

char **env_list_to_tab(env_t *head)
{
    char **new_env = malloc(sizeof(char *) * (count_exported(head) + 1));
    int i = 0;
    int len;

    for (env_t *curr = head; curr != NULL; curr = curr->next) {
        if (curr->is_exported != 1)
            continue;
        len = my_strlen(curr->name) + 2;
        if (curr->value != NULL)
            len += my_strlen(curr->value);
        new_env[i] = malloc(sizeof(char) * len);
        my_strcpy(new_env[i], curr->name);
        my_strcat(new_env[i], "=");
        if (curr->value != NULL)
            my_strcat(new_env[i], curr->value);
        i++;
    }
    new_env[i] = NULL;
    return new_env;
}
