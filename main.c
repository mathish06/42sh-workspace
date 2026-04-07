/*
** EPITECH PROJECT, 2026
** main.c, step 1
** File description:
** Step 1 of the bootstrap minishell1
*/
#include "./include/my.h"

int main(int argc, char **argv, char **env)
{
    (void)argv;
    if (argc != 1)
        return 84;
    return mysh(env);
}
