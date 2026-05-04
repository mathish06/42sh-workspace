/*
** EPITECH PROJECT, 2025
** MyProject
** File description:
** MyFile
*/

#include "../../include/my.h"

static int get_nb_len(int nb)
{
    int len = 1;

    if (nb < 0) {
        len++;
        nb = -nb;
    }
    while (nb > 9) {
        len++;
        nb /= 10;
    }
    return len;
}

char *my_itoa(int nb)
{
    int len = get_nb_len(nb);
    char *str = malloc(sizeof(char) * (len + 1));
    int is_neg = 0;

    if (str == NULL)
        return NULL;
    str[len] = '\0';
    if (nb < 0) {
        is_neg = 1;
        nb = -nb;
    }
    while (len > is_neg) {
        str[len - 1] = (nb % 10) + '0';
        nb /= 10;
        len--;
    }
    if (is_neg)
        str[0] = '-';
    return str;
}
