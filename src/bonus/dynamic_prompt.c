/*
** EPITECH PROJECT, 2026
** dynamic_prompt.c
** File description:
** dynamic_prompt for 42sh
*/
#include "../../include/my.h"

void display_dynamic_prompt(mysh_t *shell)
{
    char buffer[4096];
    char *home;

    if (getcwd(buffer, 4096) == NULL)
        return;
    if (shell->last_status == 0)
        my_putstr(VERT "[OK]" RESET);
    else
        my_putstr(ROUGE "[KO]" RESET);
    my_putstr(BLEU);
    home = my_getenv(shell->env, "HOME");
    if (home != NULL && my_strncmp(buffer, home, my_strlen(home)) == 0) {
        my_putstr("~");
        my_putstr(buffer + my_strlen(home));
    } else
        my_putstr(buffer);
    my_putstr(RESET);
    my_putstr(" $> ");
}
