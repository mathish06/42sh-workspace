/*
** EPITECH PROJECT, 2026
** my_set.c
** File description:
** builtins for local variables (set and unset)
*/

#include "../../include/my.h"

int my_unset(env_t **env, char **args)
{
    return my_unsetenv(env, args);
}

static void display_all_env(env_t *env)
{
    env_t *curr = env;

    while (curr != NULL) {
        my_putstr(curr->name);
        if (curr->value != NULL) {
            my_putchar('\t');
            my_putstr(curr->value);
        }
        my_putchar('\n');
        curr = curr->next;
    }
}

static void set_node_as_local(env_t *head)
{
    env_t *curr = head;

    if (curr == NULL)
        return;
    while (curr->next != NULL)
        curr = curr->next;
    curr->is_exported = 0;
}

int my_set(env_t **env, char **args)
{
    if (args[1] == NULL) {
        display_all_env(*env);
        return 0;
    }
    if (my_setenv(env, args) == 0) {
        set_node_as_local(*env);
    }
    return 0;
}
