/*
** EPITECH PROJECT, 2025
** my_where.c
** File description:
** my_where for 42sh
*/
#include "../../include/my.h"

static int print_alias_or_builtin_for_where(alias_t *al, char *arg)
{
    if (al != NULL) {
        my_putstr(al->name);
        my_putstr(" is aliased to ");
        my_putstr(al->value);
        my_putstr("\n");
        return 1;
    }
    if (is_builtin(arg) == 1) {
        my_putstr(arg);
        my_putstr(" is a shell built-in\n");
        return 1;
    }
    return 0;
}

static int search_all_in_paths(char **path_tab, char *cmd)
{
    char *full_path;
    int i = 0;
    int found = 0;

    if (path_tab == NULL)
        return 0;
    while (path_tab[i] != NULL) {
        full_path = try_path_composition(path_tab[i], cmd);
        if (full_path != NULL) {
            my_putstr(full_path);
            my_putstr("\n");
            free(full_path);
            found = 1;
        }
        i++;
    }
    return found;
}

static int find_all_command(char *cmd, mysh_t *shell)
{
    char *result;
    char **path_tab;
    int found_in_path;

    if (has_slash(cmd)) {
        if (access(cmd, X_OK) == 0) {
            my_putstr(cmd);
            my_putstr("\n");
            return 1;
        }
        return 0;
    }
    result = my_getenv(shell->env, "PATH");
    if (result == NULL)
        return 0;
    path_tab = my_str_to_word_array(result, ":");
    found_in_path = search_all_in_paths(path_tab, cmd);
    free_tab(path_tab);
    return found_in_path;
}

static int analyze_one_arg_for_where(mysh_t *shell, char *arg)
{
    alias_t *al = find_alias(shell->alias, arg);
    int found = 0;

    found += print_alias_or_builtin_for_where(al, arg);
    found += find_all_command(arg, shell);
    if (found == 0)
        return 1;
    return 0;
}

int my_where(mysh_t *shell, char **args)
{
    int return_value = 0;

    if (args[1] == NULL) {
        my_puterr("where: Too few arguments.\n");
        return 1;
    }
    for (int i = 1; args[i] != NULL; i++) {
        if (analyze_one_arg_for_where(shell, args[i]) == 1)
            return_value = 1;
    }
    return return_value;
}
