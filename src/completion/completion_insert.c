/*
** EPITECH PROJECT, 2026
** completion_insert.c
** File description:
** build context, free structures, splice completion into the line
*/
#include "../../include/my.h"

static char *extract_word(char *buffer, int start, int end)
{
    if (end <= start)
        return my_strdup("");
    return my_strndup(buffer + start, end - start);
}

static void split_word(comp_ctx_t *ctx, char *word)
{
    int i = my_strlen(word) - 1;

    while (i >= 0 && word[i] != '/')
        i--;
    if (i < 0) {
        ctx->dir = my_strdup(".");
        ctx->prefix = my_strdup(word);
        return;
    }
    if (i == 0)
        ctx->dir = my_strdup("/");
    else
        ctx->dir = my_strndup(word, i);
    ctx->prefix = my_strdup(word + i + 1);
}

comp_ctx_t *build_ctx(char *buffer, line_state_t *st)
{
    comp_ctx_t *ctx = malloc(sizeof(comp_ctx_t));
    char *word;

    if (ctx == NULL)
        return NULL;
    get_word_bounds(buffer, st->i, &ctx->word_start, &ctx->word_end);
    word = extract_word(buffer, ctx->word_start, ctx->word_end);
    split_word(ctx, word);
    free(word);
    return ctx;
}
