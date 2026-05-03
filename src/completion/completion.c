/*
** EPITECH PROJECT, 2026
** completion.c
** File description:
** entry point for TAB and the interactive menu loop
*/
#include "../../include/my.h"

static int menu_pick(line_ctx_t *lc, comp_ctx_t *ctx,
    comp_list_t *l, menu_state_t *m)
{
    insert_completion(lc, ctx, l->entries[m->selected],
        l->is_dir[m->selected]);
    return 0;
}
