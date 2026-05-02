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
