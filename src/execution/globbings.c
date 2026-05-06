/*
** EPITECH PROJECT, 2026
** globbing.c
** File description:
** globbing expansion for 42sh
*/
#include "../../include/my.h"
#include <glob.h>

static char **build_glob_result(glob_t *g, char **args, int orig_count)
{
    char **new_args;

    new_args = malloc(sizeof(char *) * (g->gl_pathc + 1));
    if (new_args == NULL) {
        globfree(g);
        return args;
    }
    for (size_t i = 0; i < g->gl_pathc; i++)
        new_args[i] = my_strdup(g->gl_pathv[i]);
    new_args[g->gl_pathc] = NULL;
    globfree(g);
    if ((int)g->gl_pathc != orig_count)
        free_tab(args);
    return new_args;
}

char **expand_globbing(char **args)
{
    glob_t g;
    int flags = GLOB_NOCHECK;
    int orig_count = 0;

    if (args == NULL || args[0] == NULL)
        return args;
    if (glob(args[0], flags, NULL, &g) != 0)
        return args;
    for (int i = 1; args[i] != NULL; i++) {
        glob(args[i], flags | GLOB_APPEND, NULL, &g);
    }
    while (args[orig_count] != NULL)
        orig_count++;
    return build_glob_result(&g, args, orig_count);
}
