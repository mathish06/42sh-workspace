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

static void draw_row(comp_list_t *l, menu_state_t *m, int row, int selected)
{
    int c = 0;
    int idx;

    while (c < m->cols) {
        idx = row * m->cols + c;
        if (idx >= l->count)
            break;
        draw_cell(l, idx, m, selected);
        c++;
    }
}

void compute_grid(comp_list_t *l, menu_state_t *m)
{
    int w = max_entry_len(l) + 2;

    m->term_cols = get_term_cols();
    m->col_width = w;
    m->cols = m->term_cols / w;
    if (m->cols < 1)
        m->cols = 1;
    m->rows = (l->count + m->cols - 1) / m->cols;
}

void draw_menu(comp_list_t *l, menu_state_t *m, int selected)
{
    int r = 0;

    write(1, "\n", 1);
    while (r < m->rows) {
        write(1, "\r\033[K", 4);
        draw_row(l, m, r, selected);
        if (r + 1 < m->rows)
            write(1, "\n", 1);
        r++;
    }
    r = 0;
    while (r < m->rows) {
        write(1, "\033[A", 3);
        r++;
    }
    write(1, "\r", 1);
    m->printed_rows = m->rows;
}
