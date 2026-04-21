/*
** EPITECH PROJECT, 2026
** completion_collect.c
** File description:
** read a directory and collect entries matching a prefix
*/
#include "../../include/my.h"

int starts_with(const char *str, const char *pre)
{
    int i = 0;

    while (pre[i] != '\0') {
        if (str[i] != pre[i])
            return 0;
        i++;
    }
    return 1;
}

static void push_entry(comp_list_t *l, const char *dir, const char *name)
{
    l->entries[l->count] = my_strdup((char *)name);
    l->is_dir[l->count] = is_directory(dir, name);
    l->count++;
}

comp_list_t *alloc_comp_list(int cap)
{
    comp_list_t *l = malloc(sizeof(comp_list_t));

    if (l == NULL)
        return NULL;
    l->entries = malloc(sizeof(char *) * (cap + 1));
    l->is_dir = malloc(sizeof(int) * (cap + 1));
    l->count = 0;
    if (l->entries == NULL || l->is_dir == NULL) {
        free(l->entries);
        free(l->is_dir);
        free(l);
        return NULL;
    }
    return l;
}
