/*
** EPITECH PROJECT, 2026
** history.c
** File description:
** history storage for the 42sh line editor
*/
#include "../../include/my.h"

history_t *history_init(int max)
{
    history_t *h;

    h = malloc(sizeof(history_t));
    if (h == NULL)
        return NULL;
    h->head = NULL;
    h->tail = NULL;
    h->count = 0;
    h->max = max;
    return h;
}

static void drop_oldest(history_t *h)
{
    history_entry_t *old;

    if (h->head == NULL)
        return;
    old = h->head;
    h->head = old->next;
    if (h->head != NULL)
        h->head->prev = NULL;
    else
        h->tail = NULL;
    free(old->line);
    free(old);
    h->count--;
}

static int is_duplicate_of_tail(history_t *h, const char *line)
{
    if (h->tail == NULL)
        return 0;
    if (my_strcmp(h->tail->line, (char *)line) == 0)
        return 1;
    return 0;
}

void history_add(history_t *h, const char *line)
{
    history_entry_t *entry;

    if (h == NULL || line == NULL || line[0] == '\0')
        return;
    if (is_duplicate_of_tail(h, line))
        return;
    entry = malloc(sizeof(history_entry_t));
    if (entry == NULL)
        return;
    entry->line = my_strdup(line);
    entry->next = NULL;
    entry->prev = h->tail;
    if (h->tail != NULL)
        h->tail->next = entry;
    else
        h->head = entry;
    h->tail = entry;
    h->count++;
    while (h->max > 0 && h->count > h->max)
        drop_oldest(h);
}

void history_free(history_t *h)
{
    history_entry_t *curr;
    history_entry_t *next;

    if (h == NULL)
        return;
    curr = h->head;
    while (curr != NULL) {
        next = curr->next;
        free(curr->line);
        free(curr);
        curr = next;
    }
    free(h);
}
