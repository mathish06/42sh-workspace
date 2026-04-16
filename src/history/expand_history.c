/*
** EPITECH PROJECT, 2026
** expand_history.c
** File description:
** expand history event designators (!, !!, !n, !-n, !prefix)
*/
#include "../../include/my.h"

static int gstr_append(gstr_t *g, const char *src, int n)
{
    char *tmp;
    int i;

    while (g->len + n + 1 > g->cap) {
        g->cap *= 2;
        tmp = realloc(g->buf, g->cap);
        if (tmp == NULL)
            return -1;
        g->buf = tmp;
    }
    for (i = 0; i < n; i++)
        g->buf[g->len + i] = src[i];
    g->len += n;
    g->buf[g->len] = '\0';
    return 0;
}

static int extract_token_len(const char *s)
{
    int n = 0;

    if (s[0] == '!')
        return 1;
    while (s[n] != '\0' && s[n] != ' ' && s[n] != '\t'
        && s[n] != ';' && s[n] != '|' && s[n] != '!')
        n++;
    return n;
}

static void print_event_error(const char *token, int len)
{
    int i = 0;

    write(2, "!", 1);
    while (i < len) {
        write(2, &token[i], 1);
        i++;
    }
    write(2, ": Event not found.\n", 19);
}

static int expand_one_bang(gstr_t *g, const char *line, int *i, history_t *h)
{
    int tok_len;
    history_entry_t *entry;

    tok_len = extract_token_len(&line[*i + 1]);
    if (tok_len == 0) {
        *i += 1;
        return gstr_append(g, "!", 1);
    }
    entry = resolve_event(h, &line[*i + 1], tok_len);
    if (entry == NULL) {
        print_event_error(&line[*i + 1], tok_len);
        return -1;
    }
    *i += 1 + tok_len;
    return gstr_append(g, entry->line, my_strlen(entry->line));
}

static int step_one_char(gstr_t *g, const char *line, int *i, int *quoted)
{
    if (line[*i] == '\\' && line[*i + 1] != '\0') {
        if (gstr_append(g, &line[*i], 2) < 0)
            return -1;
        *i += 2;
        return 0;
    }
    if (line[*i] == '\'')
        *quoted = !(*quoted);
    if (gstr_append(g, &line[*i], 1) < 0)
        return -1;
    *i += 1;
    return 0;
}

char *expand_history_events(const char *line, history_t *h)
{
    gstr_t g = {malloc(128), 128, 0};
    int i = 0;
    int quoted = 0;
    int rc;

    if (g.buf == NULL)
        return NULL;
    g.buf[0] = '\0';
    while (line[i] != '\0') {
        if (!quoted && line[i] == '!' && line[i + 1] != '\0')
            rc = expand_one_bang(&g, line, &i, h);
        else
            rc = step_one_char(&g, line, &i, &quoted);
        if (rc < 0) {
            free(g.buf);
            return NULL;
        }
    }
    return g.buf;
}
