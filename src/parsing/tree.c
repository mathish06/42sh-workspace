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

static token_t *find_any_redir(token_t *head)
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

static ast_node_t *create_leaf_node(token_t *head)
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

static void assign_node_type(ast_node_t *node, token_type_t type)
{
    if (type == TOKEN_SEPARATOR)
        node->type = NODE_SEPARATOR;
    if (type == TOKEN_PIPE)
        node->type = NODE_PIPE;
    if (type == TOKEN_REDIR_RIGHT)
        node->type = NODE_REDIR_R;
    if (type == TOKEN_REDIR_DOUBLE_RIGHT)
        node->type = NODE_REDIR_RR;
    if (type == TOKEN_REDIR_LEFT)
        node->type = NODE_REDIR_L;
    if (type == TOKEN_REDIR_DOUBLE_LEFT)
        node->type = NODE_REDIR_LL;
}

static ast_node_t *create_operator_node(token_t *head, token_t *split)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    token_t *curr = head;

    if (node == NULL)
        return NULL;
    assign_node_type(node, split->type);
    node->args = NULL;
    if (split != head) {
        while (curr->next != split)
            curr = curr->next;
        curr->next = NULL;
        node->left = build_ast(head);
    } else
        node->left = NULL;
    node->right = build_ast(split->next);
    if (split->value != NULL)
        free(split->value);
    free(split);
    return node;
}

static token_t *strip_parens(token_t *head)
{
    token_t *curr = head;
    token_t *prev = NULL;
    token_t *new_head;

    if (head == NULL || head->type != TOKEN_PAREN_LEFT)
        return head;
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL || curr->type != TOKEN_PAREN_RIGHT)
        return head;
    new_head = head->next;
    prev->next = NULL;
    free(curr->value);
    free(curr);
    free(head->value);
    free(head);
    return new_head;
}

static token_t *find_split_token(token_t *head)
{
    token_t *split = find_operator(head, TOKEN_SEPARATOR);

    if (split == NULL)
        split = find_operator(head, TOKEN_PIPE);
    if (split == NULL)
        split = find_any_redir(head);
    return split;
}

ast_node_t *build_ast(token_t *head)
{
    token_t *split_token = NULL;

    if (head == NULL)
        return NULL;
    split_token = find_split_token(head);
    if (split_token != NULL)
        return create_operator_node(head, split_token);
    if (head->type == TOKEN_PAREN_LEFT) {
        head = strip_parens(head);
        return build_ast(head);
    }
    return create_leaf_node(head);
}
