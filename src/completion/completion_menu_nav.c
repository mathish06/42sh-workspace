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
