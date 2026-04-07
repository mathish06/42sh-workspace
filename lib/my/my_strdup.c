/*
** EPITECH PROJECT, 2025
** MyProject
** File description:
** MyFile
*/
#include "../../include/my.h"

char *my_strdup(char const *src)
{
    char *dest;
    int len;

    if (src == NULL)
        return NULL;
    len = my_strlen(src);
    dest = malloc(sizeof(char) * (len + 1));
    if (dest == NULL)
        return NULL;
    my_strcpy(dest, src);
    return dest;
}
