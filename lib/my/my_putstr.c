/*
** EPITECH PROJECT, 2025
** my_putstr
** File description:
** task02
*/

#include "../../include/my.h"

int my_putstr(char const *str)
{
    int i;

    i = 0;
    while (str[i] != '\0') {
        my_putchar(str[i]);
        i++;
    }
    return 0;
}
