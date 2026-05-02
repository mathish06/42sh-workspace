/*
** EPITECH PROJECT, 2026
** completion_menu.c
** File description:
** draw the zsh-style grid of candidates under the prompt line
*/
#include "../../include/my.h"

static void write_padding(int n)
{
    while (n > 0) {
        write(1, " ", 1);
        n--;
    }
}

static void draw_cell(comp_list_t *l, int idx, menu_state_t *m, int selected)
{
    int len = my_strlen(l->entries[idx]);
    int pad;

    if (idx == selected)
        write(1, "\033[7m", 4);
    else if (l->is_dir[idx])
        write(1, "\033[1;36m", 7);
    write(1, l->entries[idx], len);
    if (l->is_dir[idx]) {
        write(1, "/", 1);
        len++;
    }
    write(1, "\033[0m", 4);
    pad = m->col_width - len;
    if (pad > 0)
        write_padding(pad);
}
