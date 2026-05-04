/*
** EPITECH PROJECT, 2026
** completion_first.c
** File description:
** first-word detection and dispatch to PATH command completion
*/
#include "../../include/my.h"

int already_in(comp_list_t *l, const char *name)
{
    int i = 0;

    while (i < l->count) {
        if (my_strcmp(l->entries[i], (char *)name) == 0)
            return 1;
        i++;
    }
    return 0;
}

int is_first_word(char *buffer, int word_start)
{
    int i = 0;

    while (i < word_start) {
        if (buffer[i] != ' ' && buffer[i] != '\t')
            return 0;
        i++;
    }
    return 1;
}

static int has_slash_completion(const char *s)
{
    int i = 0;

    while (s[i] != '\0') {
        if (s[i] == '/')
            return 1;
        i++;
    }
    return 0;
}

static int should_complete_command(char *buffer, comp_ctx_t *ctx)
{
    if (!is_first_word(buffer, ctx->word_start))
        return 0;
    if (has_slash_completion(ctx->prefix) || my_strcmp(ctx->dir, ".") != 0)
        return 0;
    return 1;
}

int try_path_completion(line_ctx_t *lc, comp_ctx_t *ctx)
{
    comp_list_t *l;

    if (!should_complete_command(lc->buffer, ctx))
        return 0;
    l = collect_path_commands(ctx->prefix);
    if (l == NULL || l->count == 0) {
        free_comp_list(l);
        return 0;
    }
    show_menu(lc, ctx, l);
    free_comp_list(l);
    return 1;
}
