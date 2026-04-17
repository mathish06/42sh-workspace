/*
** EPITECH PROJECT, 2026
** history_lookup.c
** File description:
** resolve a history event designator into the matching entry
*/
#include "../../include/my.h"

static int atoi_token(const char *s, int n)
{
    int value = 0;
    int i = 0;

    if (n <= 0)
        return -1;
    while (i < n) {
        if (s[i] < '0' || s[i] > '9')
            return -1;
        value = value * 10 + (s[i] - '0');
        i++;
    }
    return value;
}

static history_entry_t *find_by_index_from_head(history_t *h, int n)
{
    history_entry_t *curr = h->head;

    if (n <= 0)
        return NULL;
    while (curr != NULL && n > 1) {
        curr = curr->next;
        n--;
    }
    return curr;
}

static history_entry_t *find_by_index_from_tail(history_t *h, int n)
{
    history_entry_t *curr = h->tail;

    if (n <= 0)
        return NULL;
    while (curr != NULL && n > 1) {
        curr = curr->prev;
        n--;
    }
    return curr;
}

static int line_starts_with(const char *line, const char *pfx, int len)
{
    int i = 0;

    while (i < len) {
        if (line[i] == '\0' || line[i] != pfx[i])
            return 0;
        i++;
    }
    return 1;
}

static history_entry_t *find_by_prefix(history_t *h, const char *pfx, int len)
{
    history_entry_t *curr = h->tail;

    while (curr != NULL) {
        if (line_starts_with(curr->line, pfx, len))
            return curr;
        curr = curr->prev;
    }
    return NULL;
}

history_entry_t *resolve_event(history_t *h, const char *token, int len)
{
    int num;

    if (h == NULL || token == NULL || len <= 0)
        return NULL;
    if (len == 1 && token[0] == '!')
        return h->tail;
    if (token[0] == '-') {
        num = atoi_token(&token[1], len - 1);
        if (num < 0)
            return NULL;
        return find_by_index_from_tail(h, num);
    }
    if (token[0] >= '0' && token[0] <= '9') {
        num = atoi_token(token, len);
        if (num < 0)
            return NULL;
        return find_by_index_from_head(h, num);
    }
    return find_by_prefix(h, token, len);
}
