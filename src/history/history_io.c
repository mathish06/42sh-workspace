/*
** EPITECH PROJECT, 2026
** history_io.c
** File description:
** load and save the shell history to a file on disk
*/
#define _GNU_SOURCE
#include "../../include/my.h"
#include <stdio.h>

void history_load(history_t *h, const char *path)
{
    FILE *f;
    char *line = NULL;
    size_t n = 0;
    ssize_t got;

    if (h == NULL || path == NULL)
        return;
    f = fopen(path, "r");
    if (f == NULL)
        return;
    got = getline(&line, &n, f);
    while (got != -1) {
        if (got > 0 && line[got - 1] == '\n')
            line[got - 1] = '\0';
        if (line[0] != '\0')
            history_add(h, line);
        got = getline(&line, &n, f);
    }
    free(line);
    fclose(f);
}

void history_save(history_t *h, const char *path)
{
    FILE *f;
    history_entry_t *curr;

    if (h == NULL || path == NULL)
        return;
    f = fopen(path, "w");
    if (f == NULL)
        return;
    curr = h->head;
    while (curr != NULL) {
        fprintf(f, "%s\n", curr->line);
        curr = curr->next;
    }
    fclose(f);
}
