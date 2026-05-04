/*
** EPITECH PROJECT, 2026
** inhibitors
** File description:
** parse and search quotes
*/

#include "../../include/my.h"

int quotes_check(char *line)
{
    char current_quote = 0;
    int i = 0;

    while (line[i] != '\0') {
        if (current_quote != 0 && line[i] == current_quote) {
            current_quote = 0;
            i++;
            continue;
        }
        if (current_quote == 0 && (line[i] == '\'' || line[i] == '"'))
            current_quote = line[i];
        i++;
    }
    if (current_quote != 0) {
        my_puterr("Unmatched ");
        write(2, &current_quote, 1);
        my_puterr(".\n");
        return 0;
    }
    return 1;
}

int in_the_quotes(char *line, int pos)
{
    char current_quote = 0;
    int i = 0;

    while (i < pos) {
        if (current_quote != 0 && line[i] == current_quote) {
            current_quote = 0;
            i++;
            continue;
        }
        if (current_quote == 0 && (line[i] == '\'' || line[i] == '"'))
            current_quote = line[i];
        i++;
    }
    if (current_quote != 0)
        return 1;
    return 0;
}

char *remove_quotes(char *word)
{
    char *result = malloc(sizeof(char) * (my_strlen(word) + 1));
    int i = 0;
    int j = 0;

    if (result == NULL)
        return NULL;
    while (word[i] != '\0') {
        if (word[i] != '\'' && word[i] != '"') {
            result[j] = word[i];
            j++;
        }
        i++;
    }
    result[j] = '\0';
    return result;
}
