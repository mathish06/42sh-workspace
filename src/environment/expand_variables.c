/*
** EPITECH PROJECT, 2026
** expand_variables.c
** File description:
** expand environment and local variables ($VAR)
*/
#include "../../include/my.h"

struct expand_ctx_s {
    mysh_t *shell;
    int in_single_quote;
};

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

static int is_var_name_char(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') || c == '_';
}

static int extract_var_name_len(const char *s)
{
    int n = 0;

    while (is_var_name_char(s[n]) == 1)
        n++;
    return n;
}

static int copy_var_value(gstr_t *g, char *var_name, char *val)
{
    if (val == NULL) {
        my_puterr(var_name);
        my_puterr(": Undefined variable.\n");
        free(var_name);
        return -1;
    }
    if (gstr_append(g, val, my_strlen(val)) < 0) {
        free(var_name);
        return -1;
    }
    free(var_name);
    return 0;
}

static int expand_one_var(gstr_t *g, const char *line, int *i, mysh_t *shell)
{
    int len = extract_var_name_len(&line[*i + 1]);
    char *var_name;
    char *val;

    if (len == 0) {
        if (gstr_append(g, "$", 1) < 0)
            return -1;
        *i += 1;
        return 0;
    }
    var_name = my_strndup(&line[*i + 1], len);
    if (var_name == NULL)
        return -1;
    val = my_getenv(shell->env, var_name);
    if (copy_var_value(g, var_name, val) < 0)
        return -1;
    *i += len + 1;
    return 0;
}

static int expand_step(gstr_t *g,
    const char *line,
    int *i,
    struct expand_ctx_s *ctx)
{
    if (line[*i] == '\\' && line[*i + 1] != '\0' && ctx->in_single_quote == 0) {
        if (gstr_append(g, &line[*i], 2) < 0)
            return -1;
        *i += 2;
        return 1;
    }
    if (line[*i] == '\'') {
        ctx->in_single_quote = !ctx->in_single_quote;
        if (gstr_append(g, &line[*i], 1) < 0)
            return -1;
        *i += 1;
        return 1;
    }
    if (line[*i] == '$' && ctx->in_single_quote == 0)
        return expand_one_var(g, line, i, ctx->shell);
    if (gstr_append(g, &line[*i], 1) < 0)
        return -1;
    *i += 1;
    return 1;
}

char *expand_variable(const char *line, mysh_t *shell)
{
    gstr_t g = {malloc(128), 128, 0};
    struct expand_ctx_s ctx = {shell, 0};
    int i = 0;

    if (line == NULL || shell == NULL || g.buf == NULL)
        return NULL;
    g.buf[0] = '\0';
    while (line[i] != '\0') {
        if (expand_step(&g, line, &i, &ctx) < 0) {
            free(g.buf);
            return NULL;
        }
    }
    return g.buf;
}
