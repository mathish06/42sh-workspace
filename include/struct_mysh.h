/*
** EPITECH PROJECT, 2025
** MyProject
** File description:
** MyFile
*/
#include <termios.h>

#ifndef STRUC_H_
    #define STRUC_H_

typedef struct env_s {
    char *name;
    char *value;
    struct env_s *next;
} env_t;

typedef enum separator_e {
    SEP_NONE,
    SEP_SEMI,
    SEP_PIPE
} separator_t;

typedef enum redir_e {
    REDIR_NONE,
    REDIR_RIGHT,
    REDIR_DOUBLE_RIGHT,
    REDIR_LEFT,
    REDIR_DOUBLE_LEFT
} redir_t;

typedef struct command_s {
    char **args;
    redir_t in_type;
    char *in_file;
    redir_t out_type;
    char *out_file;
    separator_t separator;
    struct command_s *next;
} command_t;

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

typedef struct mysh_s {
    env_t *env;
    struct termios original_term;
    history_t *history;
} mysh_t;

typedef struct pipe_ctx_s {
    char **env;
    struct env_s *env_list;
} pipe_ctx_t;

#endif
