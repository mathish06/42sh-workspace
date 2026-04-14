/*
** EPITECH PROJECT, 2026
** lexer.c
** File description:
** lexer for the 42sh
*/
#include "../../include/my.h"

static token_t *create_token(token_type_t type, char *value)
{
    token_t *new_token = malloc(sizeof(token_t));

    if (new_token == NULL)
        return NULL;
    new_token->type = type;
    new_token->value = value;
    new_token->next = NULL;
    return new_token;
}

static int is_operator(char c)
{
    if (c == '|' || c == '>' || c == '<' || c == ';' || c == '(' || c == ')')
        return 1;
    return 0;
}

static token_t *handle_redirections(char *line, int *i)
{
    if (line[*i] == '>') {
        if (line[*i + 1] == '>') {
            (*i) += 2;
            return create_token(TOKEN_REDIR_DOUBLE_RIGHT, NULL);
        } else {
            (*i)++;
            return create_token(TOKEN_REDIR_RIGHT, NULL);
        }
    }
    if (line[*i] == '<') {
        if (line[*i + 1] == '<') {
            (*i) += 2;
            return create_token(TOKEN_REDIR_DOUBLE_LEFT, NULL);
        } else {
            (*i)++;
            return create_token(TOKEN_REDIR_LEFT, NULL);
        }
    }
    return NULL;
}

static token_t *handle_basic_operators(char *line, int *i)
{
    if (line[*i] == '|') {
        (*i)++;
        return create_token(TOKEN_PIPE, NULL);
    }
    if (line[*i] == ';') {
        (*i)++;
        return create_token(TOKEN_SEPARATOR, NULL);
    }
    if (line[*i] == '(') {
        (*i)++;
        return create_token(TOKEN_PAREN_LEFT, NULL);
    }
    if (line[*i] == ')') {
        (*i)++;
        return create_token(TOKEN_PAREN_RIGHT, NULL);
    }
    return NULL;
}

token_t *get_next_token(char *line, int *i)
{
    int start;
    token_t *token;

    while (line[*i] == ' ' || line[*i] == '\t')
        (*i)++;
    if (line[*i] == '\0')
        return NULL;
    token = handle_redirections(line, i);
    if (token != NULL)
        return token;
    token = handle_basic_operators(line, i);
    if (token != NULL)
        return token;
    start = (*i);
    for (; line[*i] != ' ' && line[*i] != '\t' && line[*i] != '\0'
        && is_operator(line[*i]) == 0; (*i)++);
    return create_token(TOKEN_WORD, my_strndup(&line[start], *i - start));
}

token_t *lexer(char *line)
{
    int i = 0;
    token_t *head = NULL;
    token_t *tail = NULL;
    token_t *new_token;

    while (1) {
        new_token = get_next_token(line, &i);
        if (new_token == NULL)
            break;
        if (head == NULL) {
            head = new_token;
            tail = new_token;
        } else {
            tail->next = new_token;
            tail = new_token;
        }
    }
    return head;
}
