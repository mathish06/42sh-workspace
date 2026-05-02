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
