/*
** EPITECH PROJECT, 2026
** completion_fs.c
** File description:
** small filesystem helpers used by the completion feature
*/
#include "../../include/my.h"

int is_directory(const char *dir, const char *name)
{
    char *full;
    struct stat st;
    int len;
    int rc;

    len = my_strlen((char *)dir) + my_strlen((char *)name) + 2;
    full = malloc(sizeof(char) * len);
    if (full == NULL)
        return 0;
    my_strcpy(full, (char *)dir);
    my_strcat(full, "/");
    my_strcat(full, (char *)name);
    rc = stat(full, &st);
    free(full);
    return (rc == 0 && S_ISDIR(st.st_mode));
}
