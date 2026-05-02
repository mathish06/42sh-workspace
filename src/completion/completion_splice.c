/*
** EPITECH PROJECT, 2026
** completion_splice.c
** File description:
** splice the completion text into the buffer and redraw the line
*/
#include "../../include/my.h"

static void shift_right(char *buffer, int from, int max, int by)
{
    int p = max + by - 1;

    while (p >= from + by) {
        buffer[p] = buffer[p - by];
        p--;
    }
}

static void shift_left(char *buffer, int from, int max, int by)
{
    int p = from;

    while (p + by <= max) {
        buffer[p] = buffer[p + by];
        p++;
    }
}

static void resize_word(char *buffer, line_state_t *st,
    comp_ctx_t *ctx, int new_len)
{
    int old_len = ctx->word_end - ctx->word_start;
    int diff;

    if (new_len > old_len) {
        diff = new_len - old_len;
        shift_right(buffer, ctx->word_end, st->max_len, diff);
        st->max_len += diff;
    }
    if (new_len < old_len) {
        diff = old_len - new_len;
        shift_left(buffer, ctx->word_end - diff, st->max_len, diff);
        st->max_len -= diff;
    }
    ctx->word_end = ctx->word_start + new_len;
}

static void write_new_word(char *buffer, comp_ctx_t *ctx,
    const char *text, int is_dir)
{
    int tlen = my_strlen((char *)text);
    int new_len = ctx->word_end - ctx->word_start;
    int suffix = (is_dir ? 1 : 0);
    int k = 0;

    while (k < tlen) {
        buffer[ctx->word_start + (new_len - tlen - suffix) + k] = text[k];
        k++;
    }
    if (is_dir)
        buffer[ctx->word_end - 1] = '/';
}

void insert_completion(line_ctx_t *lc, comp_ctx_t *ctx,
    const char *text, int is_dir)
{
    int old_wlen = ctx->word_end - ctx->word_start;
    int plen = my_strlen((char *)ctx->prefix);
    int tlen = my_strlen((char *)text);
    int suffix = (is_dir ? 1 : 0);
    int new_wlen = old_wlen - plen + tlen + suffix;
    int final_len = lc->st->max_len - old_wlen + new_wlen;

    if (final_len >= LINE_BUF_CAP - 1)
        return;
    resize_word(lc->buffer, lc->st, ctx, new_wlen);
    write_new_word(lc->buffer, ctx, text, is_dir);
    lc->buffer[lc->st->max_len] = '\0';
    lc->st->i = ctx->word_end;
    redraw_prompt_line(lc->buffer, lc->st);
}
