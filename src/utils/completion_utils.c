/*
** EPITECH PROJECT, 2026
** completion_utils.c
** File description:
** helpers for the completion feature (word bounds, prefix, term)
*/
#include "../../include/my.h"

void get_word_bounds(char *buffer, int i, int *start, int *end)
{
    int s = i;
    int e = i;

    while (s > 0 && buffer[s - 1] != ' ' && buffer[s - 1] != '\t')
        s--;
    while (buffer[e] != '\0' && buffer[e] != ' ' && buffer[e] != '\t')
        e++;
    *start = s;
    *end = e;
}

int get_term_cols(void)
{
    struct winsize ws;

    if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        return 80;
    return ws.ws_col;
}

int max_entry_len(comp_list_t *list)
{
    int max = 0;
    int len;
    int i = 0;

    while (i < list->count) {
        len = my_strlen(list->entries[i]);
        if (list->is_dir[i])
            len++;
        if (len > max)
            max = len;
        i++;
    }
    return max;
}

static int all_match_at(comp_list_t *list, int k, char c)
{
    int i = 1;

    while (i < list->count) {
        if (list->entries[i][k] != c)
            return 0;
        i++;
    }
    return 1;
}
