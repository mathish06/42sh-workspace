/*
** EPITECH PROJECT, 2026
** my_repeat.c
** File description:
** my_repeat for 42sh
*/
#include "../../include/my.h"

int my_repeat(mysh_t *shell, char **args)
{
    int nb = 0;
    char **sub_args = NULL;
    ast_node_t fake_node;

    if (args[1] == NULL || args[2] == NULL) {
        my_puterr("repeat: Too few arguments.\n");
        return 1;
    }
    nb = atoi(args[1]);
    if (nb <= 0)
        return 0;
    sub_args = &args[2];
    fake_node.type = NODE_COMMAND;
    fake_node.left = NULL;
    fake_node.right = NULL;
    fake_node.args = sub_args;
    for (; nb != 0; nb--) {
        exec_node_command(&fake_node, NULL, shell);
    }
    return 0;
}
