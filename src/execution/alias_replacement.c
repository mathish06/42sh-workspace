/*
** EPITECH PROJECT, 2026
** alias_replacement.c
** File description:
** alias_replacement for 42 sh
*/
#include "../../include/my.h"

static int compt_tab(char **tab)
{
    int compt = 0;
    int i = 0;

    while (tab[i] != NULL) {
        compt++;
        i++;
    }
    return (compt);
}

char **replace_args_with_alias(char **old_args, char **alias_args)
{
    int old_size = compt_tab(old_args);
    int new_size = compt_tab(alias_args);
    char **full;
    int index = 0;

    full = malloc(sizeof(char *) * (old_size - 1 + new_size + 1));
    if (full == NULL)
        return NULL;
    for (int i = 0; alias_args[i] != NULL; i++) {
        full[index] = my_strdup(alias_args[i]);
        index++;
    }
    for (int i = 1; old_args[i] != NULL; i++) {
        full[index] = my_strdup(old_args[i]);
        index++;
    }
    full[index] = NULL;
    free_tab(old_args);
    free_tab(alias_args);
    return full;
}

static char **get_alias_args(char *value)
{
    token_t *alias_tokens = lexer(value);
    char **alias_args = tokens_to_array(alias_tokens);

    free_tokens(alias_tokens);
    return alias_args;
}

static void process_alias_loop(ast_node_t *node, mysh_t *shell)
{
    char **alias_args;
    alias_t *target = NULL;
    int is_same;

    for (int loop = 0; loop < 20; loop++) {
        if (node->args[0] == NULL)
            break;
        target = find_alias(shell->alias, node->args[0]);
        if (target == NULL)
            break;
        alias_args = get_alias_args(target->value);
        if (alias_args == NULL)
            break;
        is_same = (alias_args[0] && my_strcmp(alias_args[0],
                node->args[0]) == 0);
        node->args = replace_args_with_alias(node->args, alias_args);
        if (is_same)
            break;
    }
}

void expand_aliases(ast_node_t *node, mysh_t *shell)
{
    if (node == NULL || node->args == NULL || node->args[0] == NULL)
        return;
    process_alias_loop(node, shell);
}
