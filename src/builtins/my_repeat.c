/*
** EPITECH PROJECT, 2026
** my_repeat.c
** File description:
** my_repeat for 42sh
*/
#include "../../include/my.h"

static int repeat_once(mysh_t *shell, char **args2)
{
    char **sub_args = dup_args(args2);
    ast_node_t fake_node;

    if (sub_args == NULL)
        return 1;
    fake_node.type = NODE_COMMAND;
    fake_node.left = NULL;
    fake_node.right = NULL;
    fake_node.args = sub_args;
    exec_node_command(&fake_node, NULL, shell);
    free_tab(sub_args);
    return 0;
}

int my_repeat(mysh_t *shell, char **args)
{
    int nb = 0;

    if (args[1] == NULL || args[2] == NULL) {
        my_puterr("repeat: Too few arguments.\n");
        return 1;
    }
    nb = atoi(args[1]);
    if (nb <= 0)
        return 0;
    for (; nb > 0; nb--) {
        if (repeat_once(shell, &args[2]) != 0)
            return 1;
    }
    return 0;
}
