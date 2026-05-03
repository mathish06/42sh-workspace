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
