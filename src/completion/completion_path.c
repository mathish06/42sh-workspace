/*
** EPITECH PROJECT, 2026
** completion_path.c
** File description:
** collect executables from $PATH for first-word command completion
*/
#include "../../include/my.h"

static int is_executable_in(const char *dir, const char *name)
{
    char *full;
    int len = my_strlen((char *)dir) + my_strlen((char *)name) + 2;
    int rc;

    full = malloc(sizeof(char) * len);
    if (full == NULL)
        return 0;
    my_strcpy(full, (char *)dir);
    my_strcat(full, "/");
    my_strcat(full, (char *)name);
    rc = access(full, X_OK);
    free(full);
    return (rc == 0);
}

static void add_candidate(const char *dir, const char *name, comp_list_t *l)
{
    if (already_in(l, name))
        return;
    if (!is_executable_in(dir, name))
        return;
    l->entries[l->count] = my_strdup((char *)name);
    l->is_dir[l->count] = 0;
    l->count++;
}

static void scan_path_dir(const char *dir, const char *prefix, comp_list_t *l,
    int cap)
{
    DIR *d = opendir(dir);
    struct dirent *e;

    if (d == NULL)
        return;
    e = readdir(d);
    while (e != NULL && l->count < cap) {
        if (e->d_name[0] != '.' && starts_with(e->d_name, prefix))
            add_candidate(dir, e->d_name, l);
        e = readdir(d);
    }
    closedir(d);
}
