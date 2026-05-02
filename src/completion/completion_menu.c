/*
** EPITECH PROJECT, 2026
** completion_menu.c
** File description:
** draw the zsh-style grid of candidates under the prompt line
*/
#include "../../include/my.h"

static void write_padding(int n)
{
    while (n > 0) {
        write(1, " ", 1);
        n--;
    }
}
