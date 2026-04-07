/*
** EPITECH PROJECT, 2026
** builtins.c
** File description:
** builtins for the env(s) commands
*/
#include "../include/my.h"

static int is_alphanumeric(char *str)
{
    int i = 0;
    int is_low;
    int is_up;
    int is_num;
    int is_under;

    while (str[i] != '\0') {
        is_low = (str[i] >= 'a' && str[i] <= 'z');
        is_up = (str[i] >= 'A' && str[i] <= 'Z');
        is_num = (str[i] >= '0' && str[i] <= '9');
        is_under = (str[i] == '_');
        if (!is_low && !is_up && !is_num && !is_under)
            return 0;
        i++;
    }
    return 1;
}

static int count_args(char **args)
{
    int i = 0;

    while (args[i] != NULL)
        i++;
    return i;
}

static char *create_env_string(char *key, char *value)
{
    int len = my_strlen(key) + 2;
    char *full;

    if (value != NULL)
        len += my_strlen(value);
    full = malloc(sizeof(char) * len);
    if (full == NULL)
        return NULL;
    my_strcpy(full, key);
    my_strcat(full, "=");
    if (value != NULL)
        my_strcat(full, value);
    return full;
}

int my_env(env_t *env)
{
    env_t *curr = env;

    while (curr != NULL) {
        my_putstr(curr->name);
        my_putchar('=');
        my_putstr(curr->value);
        my_putchar('\n');
        curr = curr->next;
    }
    return 0;
}

static int add_var_to_env(env_t **env, char *key, char *value)
{
    char *full = create_env_string(key, value);

    if (full == NULL)
        return 84;
    delete_env_node(env, key);
    add_node_end(env, full);
    free(full);
    return 0;
}

int my_setenv(env_t **env, char **args)
{
    int nb_args = count_args(args);

    if (nb_args == 1) {
        my_env(*env);
        return 0;
    }
    if (nb_args > 3) {
        my_putstr("setenv: Too many arguments\n");
        return 1;
    }
    if (is_alphanumeric(args[1]) == 0) {
        my_putstr("setenv: Variable name must contain"
            " alphanumeric characters.\n");
        return 1;
    }
    return add_var_to_env(env, args[1], args[2]);
}

int my_unsetenv(env_t **env, char **args)
{
    int i = 1;

    if (args[1] == NULL) {
        my_putstr("unsetenv: Too few arguments.\n");
        return 1;
    }
    while (args[i] != NULL) {
        delete_env_node(env, args[i]);
        i++;
    }
    return 0;
}
