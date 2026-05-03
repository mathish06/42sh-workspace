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

static int has_slash(const char *s)
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
    if (has_slash(ctx->prefix) || my_strcmp(ctx->dir, ".") != 0)
        return 0;
    return 1;
}
