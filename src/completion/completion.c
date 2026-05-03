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

int run_menu_loop(line_ctx_t *lc, comp_ctx_t *ctx, comp_list_t *l)
{
    menu_state_t m = {0, 0, 0, 0, 0, 0};
    int r;

    compute_grid(l, &m);
    while (1) {
        draw_menu(l, &m, m.selected);
        r = menu_navigate(l, &m);
        clear_menu(&m);
        redraw_prompt_line(lc->buffer, lc->st);
        if (r == -1)
            return 0;
        if (r == 2)
            return menu_pick(lc, ctx, l, &m);
    }
}

static int run_completion(line_ctx_t *lc, comp_ctx_t *ctx)
{
    comp_list_t *l;

    if (try_path_completion(lc, ctx) == 1)
        return 1;
    l = collect_entries(ctx);
    if (l == NULL || l->count == 0) {
        free_comp_list(l);
        return 0;
    }
    show_menu(lc, ctx, l);
    free_comp_list(l);
    return 1;
}

int handle_tab(char *buffer, line_state_t *st)
{
    line_ctx_t lc = {buffer, st};
    comp_ctx_t *ctx;
    int r;

    if (!st->interactive)
        return 0;
    ctx = build_ctx(buffer, st);
    if (ctx == NULL)
        return 0;
    r = run_completion(&lc, ctx);
    free_ctx(ctx);
    return r;
}
