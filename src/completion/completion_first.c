/*
** EPITECH PROJECT, 2026
** completion_first.c
** File description:
** first-word detection and dispatch to PATH command completion
*/
#include "../../include/my.h"

int already_in(comp_list_t *l, const char *name)
{
    int i = 0;

    while (i < l->count) {
        if (my_strcmp(l->entries[i], (char *)name) == 0)
            return 1;
        i++;
    }
    return 0;
}

int is_first_word(char *buffer, int word_start)
{
    int i = 0;

    while (i < word_start) {
        if (buffer[i] != ' ' && buffer[i] != '\t')
            return 0;
        i++;
    }
    return 1;
}
