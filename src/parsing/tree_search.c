/*
** EPITECH PROJECT, 2026
** tree_search.c
** File description:
** tree_search for the 42sh
*/
#include "../../include/my.h"

token_t *find_operator(token_t *head, token_type_t type)
{
    token_t *curr = head;
    int depth = 0;
    token_t *last_found = NULL;

    while (curr != NULL) {
        if (curr->type == TOKEN_PAREN_LEFT)
            depth++;
        if (curr->type == TOKEN_PAREN_RIGHT)
            depth--;
        if (depth == 0 && curr->type == type)
            last_found = curr;
        curr = curr->next;
    }
    return last_found;
}

token_t *find_any_redir(token_t *head)
{
    token_t *split = NULL;

    split = find_operator(head, TOKEN_REDIR_RIGHT);
    if (split == NULL)
        split = find_operator(head, TOKEN_REDIR_DOUBLE_RIGHT);
    if (split == NULL)
        split = find_operator(head, TOKEN_REDIR_LEFT);
    if (split == NULL)
        split = find_operator(head, TOKEN_REDIR_DOUBLE_LEFT);
    return split;
}

ast_node_t *create_leaf_node(token_t *head)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));

    if (node == NULL)
        return NULL;
    node->type = NODE_COMMAND;
    node->left = NULL;
    node->right = NULL;
    node->args = tokens_to_array(head);
    free_tokens(head);
    return node;
}
