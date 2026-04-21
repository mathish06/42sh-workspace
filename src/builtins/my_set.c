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

static void force_local_flag(env_t **env, char *key)
{
    env_t *curr = *env;

    while (curr != NULL) {
        if (my_strcmp(curr->name, key) == 0) {
            curr->is_exported = 0;
            return;
        }
        curr = curr->next;
    }
}

static void parse_and_set(env_t **env, char **args)
{
    char *val = NULL;
    char *fake_args[4] = {"setenv", args[1], NULL, NULL};

    if (args[2] != NULL && my_strcmp(args[2], "=") == 0)
        val = args[3];
    else if (args[2] != NULL)
        val = args[2];
    fake_args[2] = val;
    if (my_setenv(env, fake_args) == 0)
        force_local_flag(env, args[1]);
}

int my_set(env_t **env, char **args)
{
    if (args[1] == NULL) {
        display_all_env(*env);
        return 0;
    }
    parse_and_set(env, args);
    return 0;
}
