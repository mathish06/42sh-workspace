/*
** EPITECH PROJECT, 2026
** find_command.c
** File description:
** find_command for 42sh
*/
#include "../../include/my.h"

static int has_slash(char *str)
{
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '/')
            return 1;
        i++;
    }
    return 0;
}

static char *try_path_composition(char *dir, char *cmd)
{
    int len;
    char *full;

    len = my_strlen(dir) + my_strlen(cmd) + 2;
    full = malloc(sizeof(char) * len);
    if (full == NULL)
        return NULL;
    my_strcpy(full, dir);
    my_strcat(full, "/");
    my_strcat(full, cmd);
    if (access(full, X_OK) == 0)
        return full;
    free(full);
    return NULL;
}

static char *search_in_paths(char **path_tab, char *cmd)
{
    char *full_path;
    int i = 0;

    if (path_tab == NULL)
        return NULL;
    while (path_tab[i] != NULL) {
        full_path = try_path_composition(path_tab[i], cmd);
        if (full_path != NULL)
            return full_path;
        i++;
    }
    return NULL;
}

char *find_command(char *cmd, mysh_t *shell)
{
    char *result;
    char **path_tab;
    char *full_path;

    if (has_slash(cmd))
        return my_strdup(cmd);
    result = my_getenv(shell->env, "PATH");
    if (result == NULL)
        return NULL;
    path_tab = my_str_to_word_array(result, ":");
    full_path = search_in_paths(path_tab, cmd);
    free_tab(path_tab);
    return full_path;
}
