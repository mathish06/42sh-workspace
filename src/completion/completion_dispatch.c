/*
** EPITECH PROJECT, 2026
** completion_dispatch.c
** File description:
** decide between single-match, prefix-extend, and menu display
*/
#include "../../include/my.h"

int single_match(line_ctx_t *lc, comp_ctx_t *ctx, comp_list_t *l)
{
    insert_completion(lc, ctx, l->entries[0], l->is_dir[0]);
    return 0;
}
