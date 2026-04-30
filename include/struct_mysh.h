/*
** EPITECH PROJECT, 2025
** MyProject
** File description:
** MyFile
*/
#include <termios.h>

#ifndef STRUC_H_
    #define STRUC_H_

typedef enum token_type_e {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_SEPARATOR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_REDIR_RIGHT,
    TOKEN_REDIR_DOUBLE_RIGHT,
    TOKEN_REDIR_LEFT,
    TOKEN_REDIR_DOUBLE_LEFT,
    TOKEN_PAREN_LEFT,
    TOKEN_PAREN_RIGHT
} token_type_t;

typedef struct token_s {
    token_type_t type;
    char *value;
    struct token_s *next;
} token_t;

typedef enum ast_node_type_e {
    NODE_COMMAND,
    NODE_PIPE,
    NODE_SEPARATOR,
    NODE_AND,
    NODE_OR,
    NODE_REDIR_R,
    NODE_REDIR_RR,
    NODE_REDIR_L,
    NODE_REDIR_LL
} ast_node_type_t;

typedef struct ast_node_s {
    ast_node_type_t type;
    char **args;
    struct ast_node_s *left;
    struct ast_node_s *right;
} ast_node_t;

typedef struct env_s {
    char *name;
    char *value;
    int is_exported;
    struct env_s *next;
} env_t;

typedef struct history_entry_s {
    char *line;
    struct history_entry_s *next;
    struct history_entry_s *prev;
} history_entry_t;

typedef struct history_s {
    history_entry_t *head;
    history_entry_t *tail;
    int count;
    int max;
} history_t;

typedef struct line_state_s {
    int i;
    int max_len;
    int interactive;
    history_entry_t *nav_cursor;
    char *saved_draft;
} line_state_t;

typedef struct gstr_s {
    char *buf;
    int cap;
    int len;
} gstr_t;

typedef struct alias_s {
    char *name;
    char *value;
    struct alias_s *next;
} alias_t;

typedef struct mysh_s {
    env_t *env;
    struct termios original_term;
    history_t *history;
    alias_t *alias;
    int last_status;
} mysh_t;

#endif
