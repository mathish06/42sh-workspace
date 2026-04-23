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

int extend_prefix(line_ctx_t *lc, comp_ctx_t *ctx, comp_list_t *l)
{
    char *cp = common_prefix(l);
    int extended = 0;

    if (my_strlen(cp) > my_strlen(ctx->prefix)) {
        insert_completion(lc, ctx, cp, 0);
        extended = 1;
    }
    free(cp);
    return extended;
}

int show_menu(line_ctx_t *lc, comp_ctx_t *ctx, comp_list_t *l)
{
    if (l->count == 1)
        return single_match(lc, ctx, l);
    if (extend_prefix(lc, ctx, l) == 1)
        return 0;
    return run_menu_loop(lc, ctx, l);
}
