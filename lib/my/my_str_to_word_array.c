/*
** EPITECH PROJECT, 2026
** my_str_to_word_array
** File description:
** my_str_to_word_array
*/

#include "../../include/my.h"

static int my_str_isalphanum(char c, char const *separator)
{
    for (int i = 0; separator[i] != '\0'; i++) {
        if (c == separator[i])
            return 0;
    }
    return 1;
}

static int count_word(char const *str, char const *separator)
{
    int b = 1;
    int count_word = 0;

    if (!str)
        return 0;
    for (int j = 0; str[j] != 0; j++) {
        if (my_str_isalphanum(str[j], separator) == 1 && b == 1) {
            b = 0;
            count_word++;
        }
        if (my_str_isalphanum(str[j], separator) == 0 && b == 0)
            b = 1;
    }
    return (count_word);
}

static int count_decalage(char const *str, int a, char const *separator)
{
    int x = 1;

    while (my_str_isalphanum(str[a + x], separator)
        == 0 && str[a + x] != '\0') {
        x++;
    }
    return (x);
}

static char **finish(char const *str, char **dest, char const *separator)
{
    int count = 0;
    int x;

    for (int a = 0; str[a] != '\0'; a++) {
        if ((str[a + 1] == '\0'
                || my_str_isalphanum(str[a + 1], separator) == 0)
            && my_str_isalphanum(str[a], separator) == 1) {
            dest[count] = my_strndup(str, a + 1);
            count++;
            x = count_decalage(str, a, separator);
            str = str + a + x;
            a = -1;
        }
    }
    return (dest);
}

char **my_str_to_word_array(char const *str, char const *separator)
{
    char **str2 = NULL;
    int j;
    int l;

    if (!str)
        return NULL;
    j = count_word(str, separator);
    if (!j)
        return NULL;
    str2 = malloc(sizeof(char *) * (j + 1));
    if (!str2)
        return NULL;
    for (int k = 0; k <= j; k++)
        str2[k] = NULL;
    for (l = 0; str[l] != '\0' && my_str_isalphanum(str[l], separator) == 0;
        l++);
    str = str + l;
    str2 = finish(str, str2, separator);
    str2[j] = NULL;
    return str2;
}
