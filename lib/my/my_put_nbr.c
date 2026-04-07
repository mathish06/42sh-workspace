/*
** EPITECH PROJECT, 2025
** my_put_nbr
** File description:
** task07
*/

#include "../../include/my.h"

int my_put_nbr(int nb)
{
    if (nb == -2147483648) {
        my_putstr("-2147483648");
        return 0;
    }
    if (nb < 0) {
        my_putchar('-');
        nb = -nb;
    }
    if (nb >= 10) {
        my_put_nbr(nb / 10);
    }
    my_putchar('0' + (nb % 10));
    return 0;
}
