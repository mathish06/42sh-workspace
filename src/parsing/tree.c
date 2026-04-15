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

ast_node_t *build_ast(token_t *head)
{
    token_t *split_token = NULL;
    token_t *curr = head;
    ast_node_t *node;

    if (head == NULL)
        return NULL;
    split_token = find_operator(head, TOKEN_SEPARATOR);
    if (split_token == NULL)
        split_token = find_operator(head, TOKEN_PIPE);
    if (split_token != NULL) {
        node = malloc(sizeof(ast_node_t));
        if (node == NULL)
            return NULL;
        if (split_token->type == TOKEN_SEPARATOR)
            node->type = NODE_SEPARATOR;
        else
            node->type = NODE_PIPE;
        node->args = NULL;
        if (split_token != head) {
            for (; curr->next != split_token ;curr = curr->next);
            curr->next = NULL;
        }
        if (split_token == head)
            node->left = NULL;
        else
            node->left = build_ast(head);
        node->right = build_ast(split_token->next);
        return node;
    }
    node = malloc(sizeof(ast_node_t));
        if (node == NULL)
            return NULL;
    node->type = NODE_COMMAND;
    node->left = NULL;
    node->right = NULL;
    node->args = tokens_to_array(head);
    return node;
}
