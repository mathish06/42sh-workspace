/*
** EPITECH PROJECT, 2026
** completion_insert.c
** File description:
** build context, free structures, splice completion into the line
*/
#include "../../include/my.h"

static char *extract_word(char *buffer, int start, int end)
{
    if (end <= start)
        return my_strdup("");
    return my_strndup(buffer + start, end - start);
}
