/*
** EPITECH PROJECT, 2026
** inhibitors
** File description:
** parse and search quotes
*/

#include "../../include/my.h"

int quotes_check(char *line)
{
    int in_quotes = 0;
    int i = 0;

    while (line[i] != '\0') {
        if (line[i] == '\'')
            in_quotes = (in_quotes == 0) ? 1 : 0;
        i++;
    }
    if (in_quotes == 1) {
        my_putstr("Unmatched '.\n");
        return 0;
    }
    return 1;
}

int in_the_quotes(char *line, int pos)
{
    int in_quotes = 0;
    int i = 0;

    while (i < pos) {
        if (line[i] == '\'')
            in_quotes = (in_quotes == 0) ? 1 : 0;
        i++;
    }
    return in_quotes;
}

char *remove_quotes(char *word)
{
    char *result = malloc(sizeof(char) * (my_strlen(word) + 1));
    int i = 0;
    int j = 0;

    if (result == NULL)
        return NULL;
    while (word[i] != '\0') {
        if (word[i] != '\'') {
            result[j] = word[i];
            j++;
        }
        i++;
    }
    result[j] = '\0';
    return result;
}
