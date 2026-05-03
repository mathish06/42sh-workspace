/*
** EPITECH PROJECT, 2026
** utils.c
** File description:
** utils for the minishell1
*/
#include "../../include/my.h"

static void redraw_after_backspace(char *buffer, line_state_t *state)
{
    int tmp = 0;

    if (!state->interactive)
        return;
    write(1, "\b", 1);
    tmp = state->i;
    while (tmp < state->max_len) {
        write(1, &buffer[tmp], 1);
        tmp++;
    }
    write(1, " ", 1);
    tmp = state->max_len + 1;
    while (tmp > state->i) {
        write(1, "\b", 1);
        tmp--;
    }
}

static int handle_backspace(char *buffer, line_state_t *state, char c)
{
    int pos = 0;

    if (c != 127)
        return 0;
    if (state->i <= 0)
        return 1;
    state->i--;
    pos = state->i;
    while (pos < state->max_len) {
        buffer[pos] = buffer[pos + 1];
        pos++;
    }
    state->max_len--;
    redraw_after_backspace(buffer, state);
    return 1;
}

static void insert_char(char *buffer, line_state_t *state, char c)
{
    int pos = state->max_len;
    int tmp = 0;

    while (pos > state->i) {
        buffer[pos] = buffer[pos - 1];
        pos--;
    }
    buffer[state->i] = c;
    state->max_len++;
    state->i++;
    if (state->interactive) {
        for (tmp = state->i - 1; tmp < state->max_len; tmp++)
            write(1, &buffer[tmp], 1);
        while (tmp > state->i) {
            write(1, "\b", 1);
            tmp--;
        }
    }
}

static int handle_regular_char(char *buffer, line_state_t *state, char c)
{
    if (c == '\n' || c == '\r') {
        if (state->interactive)
            write(1, "\n", 1);
        buffer[state->i] = '\0';
        return 1;
    }
    if (state->i < state->max_len) {
        insert_char(buffer, state, c);
        return 0;
    }
    buffer[state->i] = c;
    state->i++;
    if (state->i > state->max_len)
        state->max_len = state->i;
    if (state->interactive)
        write(1, &c, 1);
    return 0;
}

static void handle_horizontal_arrow(line_state_t *state, char c)
{
    if (c == 'D' && state->i > 0) {
        state->i--;
        if (state->interactive)
            write(1, "\033[1D", 4);
    }
    if (c == 'C' && state->i < state->max_len) {
        state->i++;
        if (state->interactive)
            write(1, "\033[1C", 4);
    }
}

static int handle_arrows(char *buffer, line_state_t *state, char c,
    history_t *hist)
{
    if (c != 27)
        return 0;
    read(0, &c, 1);
    if (c != '[')
        return 1;
    read(0, &c, 1);
    if (c == 'A')
        history_nav_up(buffer, state, hist);
    if (c == 'B')
        history_nav_down(buffer, state, hist);
    if (c == 'C' || c == 'D')
        handle_horizontal_arrow(state, c);
    return 1;
}

static int process_input_char(char *buffer, line_state_t *state, char c,
    history_t *hist)
{
    if (c == '\t' && handle_tab(buffer, state) == 1)
        return 0;
    if (handle_backspace(buffer, state, c) == 1
        || handle_arrows(buffer, state, c, hist) == 1)
        return 0;
    if (handle_regular_char(buffer, state, c) == 1)
        return 1;
    return 0;
}

char *my_getline(history_t *hist)
{
    char *buffer = malloc(sizeof(char) * LINE_BUF_CAP);
    line_state_t state = {0, 0, isatty(0), NULL, NULL};
    char c;
    int txt = 0;

    if (buffer == NULL)
        return NULL;
    while (1) {
        txt = read(0, &c, 1);
        if (txt <= 0 || c == 4) {
            free(buffer);
            free(state.saved_draft);
            return NULL;
        }
        if (process_input_char(buffer, &state, c, hist) == 1) {
            free(state.saved_draft);
            return buffer;
        }
    }
}

int my_puterr(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        write(2, &str[i], 1);
        i++;
    }
    return 0;
}
