/*
** EPITECH PROJECT, 2026
** tree.c
** File description:
** tree for the 42sh
*/
#include "../../include/my.h"

static token_t *find_operator(token_t *head, token_type_t type)
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

static ast_node_t *create_leaf_node(token_t *head)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));

    if (node == NULL)
        return NULL;
    node->type = NODE_COMMAND;
    node->left = NULL;
    node->right = NULL;
    node->args = tokens_to_array(head);
    return node;
}

static ast_node_t *create_operator_node(token_t *head, token_t *split)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    token_t *curr = head;

    if (node == NULL)
        return NULL;
    if (split->type == TOKEN_SEPARATOR)
        node->type = NODE_SEPARATOR;
    else
        node->type = NODE_PIPE;
    node->args = NULL;
    if (split != head) {
        while (curr->next != split)
            curr = curr->next;
        curr->next = NULL;
        node->left = build_ast(head);
    } else
        node->left = NULL;
    node->right = build_ast(split->next);
    return node;
}

ast_node_t *build_ast(token_t *head)
{
    token_t *split_token = NULL;

    if (head == NULL)
        return NULL;
    split_token = find_operator(head, TOKEN_SEPARATOR);
    if (split_token == NULL)
        split_token = find_operator(head, TOKEN_PIPE);
    if (split_token != NULL)
        return create_operator_node(head, split_token);
    return create_leaf_node(head);
}
