/*
** EPITECH PROJECT, 2026
** completion_menu_nav.c
** File description:
** navigate through the completion menu with tab / arrows / enter / esc
*/
#include "../../include/my.h"

void clear_menu(menu_state_t *m)
{
    int r = 0;

    while (r < m->printed_rows) {
        write(1, "\n\r\033[K", 5);
        r++;
    }
    r = 0;
    while (r < m->printed_rows) {
        write(1, "\033[A", 3);
        r++;
    }
    write(1, "\r", 1);
    m->printed_rows = 0;
}

static int step_selection(menu_state_t *m, int count, char dir)
{
    int s = m->selected;

    if (dir == 'C' || dir == '\t')
        s = (s + 1) % count;
    if (dir == 'D')
        s = (s - 1 + count) % count;
    if (dir == 'B')
        s = (s + m->cols < count) ? s + m->cols : s;
    if (dir == 'A')
        s = (s - m->cols >= 0) ? s - m->cols : s;
    m->selected = s;
    return 0;
}

static int read_escape_seq(comp_list_t *l, menu_state_t *m)
{
    char c;

    if (read(0, &c, 1) <= 0)
        return -1;
    if (c != '[')
        return -1;
    if (read(0, &c, 1) <= 0)
        return -1;
    step_selection(m, l->count, c);
    return 1;
}

static int dispatch_key(comp_list_t *l, menu_state_t *m, char c)
{
    if (c == '\n' || c == '\r')
        return 2;
    if (c == 27)
        return read_escape_seq(l, m);
    if (c == '\t') {
        step_selection(m, l->count, '\t');
        return 1;
    }
    return -1;
}
