/*
** EPITECH PROJECT, 2026
** error_handling.c
** File description:
** error_handling for the minishell1
*/
#include "../include/my.h"

void print_exec_error(char *cmd)
{
    if (errno == ENOEXEC) {
        my_putstr(cmd);
        my_putstr(": Exec format error. Binary file not executable.\n");
        return;
    }
    if (errno == EACCES) {
        my_putstr(cmd);
        my_putstr(": Permission denied.\n");
        return;
    }
    if (errno == ENOENT) {
        my_putstr(cmd);
        my_putstr(": Command not found.\n");
        return;
    }
    perror(cmd);
}
