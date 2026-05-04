/*
** EPITECH PROJECT, 2026
** completion_draw.c
** File description:
** redraw the prompt line after a completion or menu close
*/
#include "../../include/my.h"

void redraw_prompt_line(char *buffer, line_state_t *st)
{
    int n = st->max_len - st->i;

    if (!st->interactive)
        return;
    write(1, "\r\033[K$> ", 7);
    write(1, buffer, st->max_len);
    while (n > 0) {
        write(1, "\b", 1);
        n--;
    }
}
