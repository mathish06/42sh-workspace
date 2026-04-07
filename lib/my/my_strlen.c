/*
** EPITECH PROJECT, 2025
** my_strlen
** File description:
** task03
*/

int my_strlen(char const *str)
{
    int compt = 0;
    int i = 0;

    while (str[i] != '\0') {
        compt++;
        i++;
    }
    return (compt);
}
