/*
** EPITECH PROJECT, 2026
** reception.c
** File description:
** reception.c for 42sh
*/
#include "../../include/my.h"

void display_greeting(void)
{
    my_putstr(CYAN);
    my_putstr("  _  _   ___      _     \n");
    my_putstr(" | || | |__ \\    | |    \n");
    my_putstr(" | || |_   ) |___| |__  \n");
    my_putstr(" |__   _| / // __| '_ \\ \n");
    my_putstr("    | |  / /_\\__ \\ | | |\n");
    my_putstr("    |_| |____|___/_| |_|\n");
    my_putstr(RESET);
    my_putstr("\n");
    my_putstr(MAGENTA);
    my_putstr("Bienvenue sur le système !");
    my_putstr("\n");
    my_putstr(RESET);
}
