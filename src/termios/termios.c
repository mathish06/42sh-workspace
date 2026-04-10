/*
** EPITECH PROJECT, 2026
** termios.c
** File description:
** termios for the 42sh
*/
#include "../../include/my.h"

void enable_raw_mode(struct termios *original_term)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, original_term);
    raw = *original_term;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(struct termios *original_term)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original_term);
}
