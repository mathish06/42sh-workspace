/*
** EPITECH PROJECT, 2026
** completion_collect.c
** File description:
** read a directory and collect entries matching a prefix
*/
#include "../../include/my.h"

int starts_with(const char *str, const char *pre)
{
    int i = 0;

    while (pre[i] != '\0') {
        if (str[i] != pre[i])
            return 0;
        i++;
    }
    return 1;
}
