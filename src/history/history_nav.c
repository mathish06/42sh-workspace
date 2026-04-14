/*
** EPITECH PROJECT, 2026
** history_nav.c
** File description:
** up/down arrow navigation through the history entries
*/
#include "../../include/my.h"

static void replace_buffer(char *buffer, line_state_t *st, const char *src)
{
    int len;

    len = my_strlen((char *)src);
    my_strncpy(buffer, (char *)src, len);
    buffer[len] = '\0';
    st->i = len;
    st->max_len = len;
}

static void redraw_line(char *buffer, line_state_t *st)
{
    if (!st->interactive)
        return;
    write(1, "\r\033[K$> ", 7);
    write(1, buffer, st->max_len);
}

static void save_draft_if_needed(char *buffer, line_state_t *st)
{
    if (st->nav_cursor != NULL)
        return;
    if (st->saved_draft != NULL)
        free(st->saved_draft);
    st->saved_draft = my_strndup(buffer, st->max_len);
}

void history_nav_up(char *buffer, line_state_t *st, history_t *h)
{
    history_entry_t *target;

    if (h == NULL || h->tail == NULL)
        return;
    if (st->nav_cursor == NULL)
        target = h->tail;
    else if (st->nav_cursor->prev != NULL)
        target = st->nav_cursor->prev;
    else
        return;
    save_draft_if_needed(buffer, st);
    st->nav_cursor = target;
    replace_buffer(buffer, st, target->line);
    redraw_line(buffer, st);
}

void history_nav_down(char *buffer, line_state_t *st, history_t *h)
{
    (void)h;
    if (st->nav_cursor == NULL)
        return;
    if (st->nav_cursor->next != NULL) {
        st->nav_cursor = st->nav_cursor->next;
        replace_buffer(buffer, st, st->nav_cursor->line);
    } else {
        st->nav_cursor = NULL;
        if (st->saved_draft != NULL)
            replace_buffer(buffer, st, st->saved_draft);
        else
            replace_buffer(buffer, st, "");
    }
    redraw_line(buffer, st);
}
