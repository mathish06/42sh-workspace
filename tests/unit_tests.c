#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../include/my.h"

void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(my_putchar, basic_char, .init = redirect_all_std)
{
    my_putchar('c');
    cr_assert_stdout_eq_str("c");
}

Test(my_putstr, basic_string, .init = redirect_all_std)
{
    my_putstr("Hello");
    cr_assert_stdout_eq_str("Hello");
}

Test(my_put_nbr, basic_numbers, .init = redirect_all_std)
{
    my_put_nbr(42);
    my_put_nbr(-42);
    my_put_nbr(0);
    my_put_nbr(-2147483648);
    cr_assert_stdout_eq_str("42-420-2147483648");
}

Test(my_strlen, basic_length)
{
    cr_assert_eq(my_strlen("Hello"), 5);
    cr_assert_eq(my_strlen(""), 0);
}

Test(my_strcmp, basic_comparison)
{
    cr_assert_eq(my_strcmp("abc", "abc"), 0);
    cr_assert_gt(my_strcmp("abd", "abc"), 0);
    cr_assert_lt(my_strcmp("aba", "abc"), 0);
}

Test(my_strcpy, basic_copy)
{
    char dest[10];
    my_strcpy(dest, "Hello");
    cr_assert_str_eq(dest, "Hello");
}

Test(my_strncpy, basic_n_copy)
{
    char dest[10] = "XXXXXXXXX";
    my_strncpy(dest, "Hello", 3);
    cr_assert_str_eq(dest, "HelXXXXXX");
}

Test(my_strcat, basic_concat)
{
    char dest[20] = "Hello ";
    my_strcat(dest, "World");
    cr_assert_str_eq(dest, "Hello World");
}

Test(my_strdup, basic_dup)
{
    char *str = my_strdup("Hello");
    cr_assert_str_eq(str, "Hello");
    free(str);
    cr_assert_null(my_strdup(NULL));
}

Test(my_strndup, basic_n_dup)
{
    char *str = my_strndup("Hello", 3);
    cr_assert_str_eq(str, "Hel");
    free(str);
}

Test(my_str_to_word_array, basic_split)
{
    char **tab = my_str_to_word_array("Hello World 42", " ");
    cr_assert_str_eq(tab[0], "Hello");
    cr_assert_str_eq(tab[1], "World");
    cr_assert_str_eq(tab[2], "42");
    cr_assert_null(tab[3]);
    free_tab(tab);
    cr_assert_null(my_str_to_word_array(NULL, " "));
}

Test(env_list, create_and_delete)
{
    char *envp[] = {"USER=loan", "PATH=/bin", NULL};
    env_t *env_list = env_to_list(envp);
    cr_assert_not_null(env_list);
    cr_assert_str_eq(env_list->name, "USER");
    cr_assert_str_eq(env_list->value, "loan");
    cr_assert_str_eq(my_getenv(env_list, "PATH"), "/bin");
    cr_assert_null(my_getenv(env_list, "TOTO"));
    delete_env_node(&env_list, "USER");
    cr_assert_null(my_getenv(env_list, "USER"));
    free_env_list(env_list);
}

Test(builtins_env, my_env, .init = redirect_all_std)
{
    char *envp[] = {"USER=loan", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;

    shell.env = env_list;
    shell.alias = NULL;
    my_env(&shell);
    cr_assert_stdout_eq_str("USER=loan\n");
    free_env_list(shell.env);
}

Test(builtins_setenv, basic_setenv, .init = redirect_all_std)
{
    char *envp[] = {"USER=loan", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;
    char *args[] = {"setenv", "TEST", "value", NULL};
    char *args_err[] = {"setenv", "T-EST", "value", NULL};
    char *args_many[] = {"setenv", "1", "2", "3", NULL};
    char *args_none[] = {"setenv", NULL};

    shell.env = env_list;
    shell.alias = NULL;
    my_setenv(&shell, args);
    cr_assert_str_eq(my_getenv(shell.env, "TEST"), "value");
    my_setenv(&shell, args_err);
    my_setenv(&shell, args_many);
    my_setenv(&shell, args_none);
    free_env_list(shell.env);
}

Test(builtins_unsetenv, basic_unsetenv, .init = redirect_all_std)
{
    char *envp[] = {"USER=loan", "TEST=value", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;
    char *args[] = {"unsetenv", "TEST", NULL};
    char *args_err[] = {"unsetenv", NULL};

    shell.env = env_list;
    shell.alias = NULL;
    my_unsetenv(&shell, args);
    cr_assert_null(my_getenv(shell.env, "TEST"));
    my_unsetenv(&shell, args_err);
    free_env_list(shell.env);
}

Test(builtins_cd, basic_cd, .init = redirect_all_std)
{
    char *envp[] = {"HOME=/tmp", "OLDPWD=/tmp", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;
    char *args_tmp[] = {"cd", "/tmp", NULL};
    char *args_home[] = {"cd", NULL};
    char *args_dash[] = {"cd", "-", NULL};
    char *args_many[] = {"cd", "1", "2", NULL};
    char *args_err[] = {"cd", "/impossible_directory_42", NULL};

    shell.env = env_list;
    shell.alias = NULL;
    cr_assert_eq(my_cd(args_tmp, &shell), 0);
    cr_assert_eq(my_cd(args_home, &shell), 0);
    cr_assert_eq(my_cd(args_dash, &shell), 0);
    cr_assert_eq(my_cd(args_many, &shell), 1);
    cr_assert_eq(my_cd(args_err, &shell), 1);
    free_env_list(shell.env);
}

Test(error_handling, print_errors, .init = redirect_all_std)
{
    errno = ENOEXEC;
    print_exec_error("test_cmd");
    errno = EACCES;
    print_exec_error("test_cmd");
    errno = ENOENT;
    print_exec_error("test_cmd");
}

Test(exec_command, find_command)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;
    char *path;
    char *abs_path;

    shell.env = env_list;
    shell.alias = NULL;
    path = find_command("ls", &shell);
    abs_path = find_command("/bin/ls", &shell);
    cr_assert_not_null(path);
    free(path);
    cr_assert_str_eq(abs_path, "/bin/ls");
    free(abs_path);
    free_env_list(shell.env);
}

Test(mysh_main, test_exit_and_commands)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    int saved_stdin = dup(0);
    int pipefd[2];

    pipe(pipefd);
    write(pipefd[1], "ls\necho hello\nexit\n", 19);
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
    mysh(envp);
    dup2(saved_stdin, 0);
    close(saved_stdin);
}

Test(my_strncpy, padding_null_bytes)
{
    char dest[10] = "XXXXXXXXX";

    my_strncpy(dest, "a", 5);
    cr_assert_eq(dest[1], '\0');
    cr_assert_eq(dest[4], '\0');
}

Test(mysh_main, test_eof_ctrl_d, .init = redirect_all_std)
{
    char *envp[] = {"PATH=/bin", NULL};
    int saved_stdin = dup(0);
    int pipefd[2];

    pipe(pipefd);
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
    mysh(envp);
    dup2(saved_stdin, 0);
    close(saved_stdin);
}

Test(history_store, init_empty)
{
    history_t *h = history_init(10);

    cr_assert_not_null(h);
    cr_assert_eq(h->count, 0);
    cr_assert_null(h->head);
    cr_assert_null(h->tail);
    history_free(h);
}

Test(history_store, add_and_order)
{
    history_t *h = history_init(10);

    history_add(h, "first");
    history_add(h, "second");
    history_add(h, "third");
    cr_assert_eq(h->count, 3);
    cr_assert_str_eq(h->head->line, "first");
    cr_assert_str_eq(h->tail->line, "third");
    history_free(h);
}

Test(history_store, ignore_duplicate_tail)
{
    history_t *h = history_init(10);

    history_add(h, "same");
    history_add(h, "same");
    history_add(h, "same");
    cr_assert_eq(h->count, 1);
    history_free(h);
}

Test(history_store, ignore_empty_and_null)
{
    history_t *h = history_init(10);

    history_add(h, "");
    history_add(h, NULL);
    history_add(NULL, "x");
    cr_assert_eq(h->count, 0);
    history_free(h);
}

Test(history_store, cap_at_max)
{
    history_t *h = history_init(3);
    int i = 0;
    char buf[8];

    while (i < 6) {
        buf[0] = 'a' + i;
        buf[1] = '\0';
        history_add(h, buf);
        i++;
    }
    cr_assert_eq(h->count, 3);
    cr_assert_str_eq(h->head->line, "d");
    cr_assert_str_eq(h->tail->line, "f");
    history_free(h);
}

Test(history_store, save_and_load_round_trip)
{
    history_t *h = history_init(10);
    history_t *h2;
    const char *path = "/tmp/.42sh_unit_hist";

    history_add(h, "alpha");
    history_add(h, "beta gamma");
    history_save(h, path);
    h2 = history_init(10);
    history_load(h2, path);
    cr_assert_eq(h2->count, 2);
    cr_assert_str_eq(h2->head->line, "alpha");
    cr_assert_str_eq(h2->tail->line, "beta gamma");
    unlink(path);
    history_free(h);
    history_free(h2);
}

Test(history_store, load_missing_file_noop)
{
    history_t *h = history_init(10);

    history_load(h, "/tmp/.42sh_does_not_exist_ever_xyz");
    cr_assert_eq(h->count, 0);
    history_free(h);
}

Test(history_expand, no_bang_returns_copy)
{
    history_t *h = history_init(10);
    char *out = expand_history_events("echo hello", h);

    cr_assert_not_null(out);
    cr_assert_str_eq(out, "echo hello");
    free(out);
    history_free(h);
}

Test(history_expand, bang_bang_last)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "ls -la");
    out = expand_history_events("!!", h);
    cr_assert_not_null(out);
    cr_assert_str_eq(out, "ls -la");
    free(out);
    history_free(h);
}

Test(history_expand, bang_n_from_head)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "one");
    history_add(h, "two");
    history_add(h, "three");
    out = expand_history_events("!1", h);
    cr_assert_str_eq(out, "one");
    free(out);
    history_free(h);
}

Test(history_expand, bang_minus_n_from_tail)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "one");
    history_add(h, "two");
    history_add(h, "three");
    out = expand_history_events("!-2", h);
    cr_assert_str_eq(out, "two");
    free(out);
    history_free(h);
}

Test(history_expand, bang_prefix_most_recent)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "echo alpha");
    history_add(h, "ls");
    history_add(h, "echo bravo");
    out = expand_history_events("!ec", h);
    cr_assert_str_eq(out, "echo bravo");
    free(out);
    history_free(h);
}

Test(history_expand, bang_not_found_returns_null, .init = redirect_all_std)
{
    history_t *h = history_init(10);
    char *out = expand_history_events("!nope", h);

    cr_assert_null(out);
    history_free(h);
}

Test(history_expand, bang_inside_single_quotes_preserved)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "ls");
    out = expand_history_events("echo '!!'", h);
    cr_assert_str_eq(out, "echo '!!'");
    free(out);
    history_free(h);
}

Test(history_expand, bang_escaped_with_backslash_preserved)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "ls");
    out = expand_history_events("echo \\!!", h);
    cr_assert_str_eq(out, "echo \\!!");
    free(out);
    history_free(h);
}

Test(history_expand, bang_concat_with_trailing_text)
{
    history_t *h = history_init(10);
    char *out;

    history_add(h, "ls -la");
    out = expand_history_events("!! | cat", h);
    cr_assert_str_eq(out, "ls -la | cat");
    free(out);
    history_free(h);
}

Test(inhibitors, quotes_check_valid)
{
    cr_assert_eq(quotes_check("echo hello"), 1);
    cr_assert_eq(quotes_check("echo 'hello'"), 1);
    cr_assert_eq(quotes_check("echo \"hello\""), 1);
    cr_assert_eq(quotes_check("echo 'a' \"b\""), 1);
    cr_assert_eq(quotes_check(""), 1);
}

Test(inhibitors, quotes_check_unmatched, .init = redirect_all_std)
{
    cr_assert_eq(quotes_check("echo 'hello"), 0);
    cr_assert_eq(quotes_check("echo \"hello"), 0);
    cr_assert_eq(quotes_check("'"), 0);
    cr_assert_eq(quotes_check("\""), 0);
    cr_assert_eq(quotes_check("echo 'a' \"b"), 0);
}

Test(inhibitors, in_the_quotes_detection)
{
    char *line = "echo 'hello' \"world\"";

    cr_assert_eq(in_the_quotes(line, 0), 0);
    cr_assert_eq(in_the_quotes(line, 5), 0);
    cr_assert_eq(in_the_quotes(line, 7), 1);
    cr_assert_eq(in_the_quotes(line, 12), 0);
    cr_assert_eq(in_the_quotes(line, 15), 1);
    cr_assert_eq(in_the_quotes(line, 20), 0);
}

Test(inhibitors, in_the_quotes_no_quotes)
{
    char *line = "echo hello world";

    cr_assert_eq(in_the_quotes(line, 0), 0);
    cr_assert_eq(in_the_quotes(line, 5), 0);
    cr_assert_eq(in_the_quotes(line, 10), 0);
}

Test(inhibitors, remove_quotes_basic)
{
    char *result_single = remove_quotes("'hello'");
    char *result_double = remove_quotes("\"hello\"");

    cr_assert_str_eq(result_single, "hello");
    cr_assert_str_eq(result_double, "hello");
    
    free(result_single);
    free(result_double);
}

Test(inhibitors, remove_quotes_no_quotes)
{
    char *result = remove_quotes("hello");

    cr_assert_str_eq(result, "hello");
    free(result);
}

Test(inhibitors, remove_quotes_mixed_content)
{
    char *result = remove_quotes("hello'world'\"test\"");

    cr_assert_str_eq(result, "helloworldtest");
    free(result);
}

Test(builtins_set, basic_set_and_unset, .init = redirect_all_std)
{
    char *envp[] = {"USER=loan", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;
    env_t *curr;
    char *args_set[] = {"set", "LOCAL_VAR", "42", NULL};
    char *args_unset[] = {"unset", "LOCAL_VAR", NULL};

    shell.env = env_list;
    shell.alias = NULL;
    my_set(&shell, args_set);
    cr_assert_str_eq(my_getenv(shell.env, "LOCAL_VAR"), "42");
    curr = shell.env;
    while (curr != NULL && my_strcmp(curr->name, "LOCAL_VAR") != 0)
        curr = curr->next;
    cr_assert_not_null(curr);
    cr_assert_eq(curr->is_exported, 0);
    my_unset(&shell, args_unset);
    cr_assert_null(my_getenv(shell.env, "LOCAL_VAR"));
    free_env_list(shell.env);
}

Test(utils, env_list_to_tab)
{
    char *envp[] = {"GLOBAL1=val1", NULL};
    env_t *env_list = env_to_list(envp);
    mysh_t shell;
    char **env_tab;
    char *args_set[] = {"set", "LOCAL1", "val2", NULL};

    shell.env = env_list;
    shell.alias = NULL;
    my_set(&shell, args_set);
    env_tab = env_list_to_tab(shell.env);
    cr_assert_not_null(env_tab);
    cr_assert_str_eq(env_tab[0], "GLOBAL1=val1");
    cr_assert_null(env_tab[1]);
    free_tab(env_tab);
    free_env_list(shell.env);
}

Test(alias_list, add_and_find_basic)
{
    alias_t *list = NULL;
    alias_t *target = NULL;

    add_alias(&list, "ll", "ls -l");
    target = find_alias(list, "ll");
    cr_assert_not_null(target);
    cr_assert_str_eq(target->name, "ll");
    cr_assert_str_eq(target->value, "ls -l");
    free_alias_list(list);
}

Test(alias_list, overwrite_existing_alias)
{
    alias_t *list = NULL;
    alias_t *target = NULL;

    add_alias(&list, "ll", "ls -l");
    add_alias(&list, "ll", "ls -la --color");
    target = find_alias(list, "ll");
    cr_assert_not_null(target);
    cr_assert_str_eq(target->value, "ls -la --color");
    free_alias_list(list);
}

Test(alias_list, find_non_existent)
{
    alias_t *list = NULL;
    alias_t *target = NULL;

    add_alias(&list, "ll", "ls -l");
    target = find_alias(list, "mdr");
    cr_assert_null(target);
    free_alias_list(list);
}

Test(alias_list, delete_alias)
{
    alias_t *list = NULL;

    add_alias(&list, "ll", "ls -l");
    add_alias(&list, "grep", "grep --color");
    delete_alias(&list, "ll");
    cr_assert_null(find_alias(list, "ll"));
    cr_assert_not_null(find_alias(list, "grep"));
    delete_alias(&list, "grep");
    cr_assert_null(list);
    delete_alias(&list, "fantome");
    free_alias_list(list);
}

Test(builtin_alias, my_alias_creation)
{
    mysh_t shell;
    char *args[] = {"alias", "ll", "ls", "-l", "--color=auto", NULL};
    alias_t *target;

    shell.alias = NULL;
    my_alias(&shell, args);
    target = find_alias(shell.alias, "ll");
    cr_assert_not_null(target);
    cr_assert_str_eq(target->value, "ls -l --color=auto");
    free_alias_list(shell.alias);
}

Test(builtin_unalias, my_unalias_basic)
{
    mysh_t shell;
    shell.alias = NULL;
    char *args[] = {"unalias", "ll", NULL};

    add_alias(&shell.alias, "ll", "ls -l");
    add_alias(&shell.alias, "grep", "grep --color");

    my_unalias(&shell, args);
    
    cr_assert_null(find_alias(shell.alias, "ll"));
    cr_assert_not_null(find_alias(shell.alias, "grep"));

    free_alias_list(shell.alias);
}

Test(alias_replacement, replace_args_with_alias_basic)
{
    char **old_args = malloc(sizeof(char *) * 3);
    char **alias_args = malloc(sizeof(char *) * 3);
    char **new_args;

    old_args[0] = my_strdup("ll");
    old_args[1] = my_strdup("/tmp");
    old_args[2] = NULL;

    alias_args[0] = my_strdup("ls");
    alias_args[1] = my_strdup("-l");
    alias_args[2] = NULL;

    new_args = replace_args_with_alias(old_args, alias_args);

    cr_assert_not_null(new_args);
    cr_assert_str_eq(new_args[0], "ls");
    cr_assert_str_eq(new_args[1], "-l");
    cr_assert_str_eq(new_args[2], "/tmp");
    cr_assert_null(new_args[3]);

    free_tab(new_args);
}

Test(alias_replacement, expand_aliases_basic)
{
    mysh_t shell;
    ast_node_t node;

    shell.alias = NULL;
    add_alias(&shell.alias, "ll", "ls -l");

    node.args = malloc(sizeof(char *) * 3);
    node.args[0] = my_strdup("ll");
    node.args[1] = my_strdup("/tmp");
    node.args[2] = NULL;

    expand_aliases(&node, &shell);

    cr_assert_str_eq(node.args[0], "ls");
    cr_assert_str_eq(node.args[1], "-l");
    cr_assert_str_eq(node.args[2], "/tmp");
    cr_assert_null(node.args[3]);

    free_tab(node.args);
    free_alias_list(shell.alias);
}

Test(alias_replacement, expand_aliases_recursive)
{
    mysh_t shell;
    ast_node_t node;

    shell.alias = NULL;
    add_alias(&shell.alias, "a", "b");
    add_alias(&shell.alias, "b", "ls -l");

    node.args = malloc(sizeof(char *) * 2);
    node.args[0] = my_strdup("a");
    node.args[1] = NULL;

    expand_aliases(&node, &shell);

    cr_assert_str_eq(node.args[0], "ls");
    cr_assert_str_eq(node.args[1], "-l");
    cr_assert_null(node.args[2]);

    free_tab(node.args);
    free_alias_list(shell.alias);
}

Test(alias_replacement, expand_aliases_loop_protection)
{
    mysh_t shell;
    ast_node_t node;

    shell.alias = NULL;
    add_alias(&shell.alias, "ls", "ls --color");

    node.args = malloc(sizeof(char *) * 2);
    node.args[0] = my_strdup("ls");
    node.args[1] = NULL;

    expand_aliases(&node, &shell);

    cr_assert_str_eq(node.args[0], "ls");
    cr_assert_str_eq(node.args[1], "--color");
    cr_assert_null(node.args[2]);

    free_tab(node.args);
    free_alias_list(shell.alias);
}

Test(alias_replacement, expand_aliases_with_quotes)
{
    mysh_t shell;
    ast_node_t node;

    shell.alias = NULL;
    add_alias(&shell.alias, "msg", "echo \"hello world\"");

    node.args = malloc(sizeof(char *) * 2);
    node.args[0] = my_strdup("msg");
    node.args[1] = NULL;

    expand_aliases(&node, &shell);

    cr_assert_str_eq(node.args[0], "echo");
    cr_assert_str_eq(node.args[1], "hello world");
    cr_assert_null(node.args[2]);

    free_tab(node.args);
    free_alias_list(shell.alias);
}

Test(alias_replacement, expand_aliases_null_safety)
{
    mysh_t shell;
    ast_node_t node;

    shell.alias = NULL;
    node.args = NULL;

    expand_aliases(&node, &shell);
    expand_aliases(NULL, &shell);
    
    cr_assert(1);
}

Test(exec_pipe, basic_echo_cat, .init = redirect_all_std)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    mysh_t shell;
    ast_node_t left_node;
    ast_node_t right_node;
    ast_node_t pipe_node;
    char *args_left[] = {"echo", "criterion_pipe_test", NULL};
    char *args_right[] = {"cat", NULL};

    shell.env = env_to_list(envp);
    shell.alias = NULL;

    left_node.type = NODE_COMMAND;
    left_node.args = args_left;
    left_node.left = NULL;
    left_node.right = NULL;

    right_node.type = NODE_COMMAND;
    right_node.args = args_right;
    right_node.left = NULL;
    right_node.right = NULL;

    pipe_node.type = NODE_PIPE;
    pipe_node.args = NULL;
    pipe_node.left = &left_node;
    pipe_node.right = &right_node;

    exec_pipe_node(&pipe_node, envp, &shell);

    cr_assert_stdout_eq_str("criterion_pipe_test\n");

    free_env_list(shell.env);
}

Test(exec_pipe, builtin_and_system_cmd, .init = redirect_all_std)
{
    char *envp[] = {"MY_CUSTOM_VAR=pipe_secret", "PATH=/bin:/usr/bin", NULL};
    mysh_t shell;
    ast_node_t left_node;
    ast_node_t right_node;
    ast_node_t pipe_node;
    char *args_left[] = {"env", NULL};
    char *args_right[] = {"grep", "MY_CUSTOM_VAR", NULL};

    shell.env = env_to_list(envp);
    shell.alias = NULL;

    left_node.type = NODE_COMMAND;
    left_node.args = args_left;
    left_node.left = NULL;
    left_node.right = NULL;

    right_node.type = NODE_COMMAND;
    right_node.args = args_right;
    right_node.left = NULL;
    right_node.right = NULL;

    pipe_node.type = NODE_PIPE;
    pipe_node.args = NULL;
    pipe_node.left = &left_node;
    pipe_node.right = &right_node;

    exec_pipe_node(&pipe_node, envp, &shell);

    cr_assert_stdout_eq_str("MY_CUSTOM_VAR=pipe_secret\n");

    free_env_list(shell.env);
}

Test(exec_redir, redir_right_and_double_right)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    mysh_t shell;
    char *args_cmd[] = {"echo", "first_line", NULL};
    char *args_file[] = {"/tmp/crit_out.txt", NULL};
    ast_node_t cmd_node = {NODE_COMMAND, args_cmd, NULL, NULL};
    ast_node_t file_node = {NODE_COMMAND, args_file, NULL, NULL};
    ast_node_t redir_node = {NODE_REDIR_R, NULL, &cmd_node, &file_node};
    int fd;
    char buffer[100] = {0};

    shell.env = env_to_list(envp);
    shell.alias = NULL;

    unlink("/tmp/crit_out.txt");

    exec_redir_node(&redir_node, envp, &shell);

    args_cmd[1] = "second_line";
    redir_node.type = NODE_REDIR_RR;
    exec_redir_node(&redir_node, envp, &shell);

    fd = open("/tmp/crit_out.txt", O_RDONLY);
    cr_assert_neq(fd, -1, "Le fichier de redirection n'a pas été créé.");
    
    read(fd, buffer, 99);
    close(fd);

    cr_assert_str_eq(buffer, "first_line\nsecond_line\n");

    unlink("/tmp/crit_out.txt");
    free_env_list(shell.env);
}

Test(exec_redir, redir_left, .init = redirect_all_std)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    mysh_t shell;
    int fd;
    char *args_cmd[] = {"cat", NULL};
    char *args_file[] = {"/tmp/crit_in.txt", NULL};
    ast_node_t cmd_node = {NODE_COMMAND, args_cmd, NULL, NULL};
    ast_node_t file_node = {NODE_COMMAND, args_file, NULL, NULL};
    ast_node_t redir_node = {NODE_REDIR_L, NULL, &cmd_node, &file_node};

    shell.env = env_to_list(envp);
    shell.alias = NULL;

    fd = open("/tmp/crit_in.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "hello_left\n", 11);
    close(fd);

    exec_redir_node(&redir_node, envp, &shell);

    cr_assert_stdout_eq_str("hello_left\n");

    unlink("/tmp/crit_in.txt");
    free_env_list(shell.env);
}

Test(exec_redir, redir_double_left_heredoc, .init = redirect_all_std)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    mysh_t shell;
    int pipefd[2];
    int saved_stdin;
    char *args_cmd[] = {"cat", NULL};
    char *args_file[] = {"EOF", NULL};
    ast_node_t cmd_node = {NODE_COMMAND, args_cmd, NULL, NULL};
    ast_node_t file_node = {NODE_COMMAND, args_file, NULL, NULL};
    ast_node_t redir_node = {NODE_REDIR_LL, NULL, &cmd_node, &file_node};

    shell.env = env_to_list(envp);
    shell.alias = NULL;

    pipe(pipefd);
    write(pipefd[1], "heredoc_line\nEOF\n", 17);
    close(pipefd[1]);
    
    saved_stdin = dup(0);
    dup2(pipefd[0], 0);
    close(pipefd[0]);

    exec_redir_node(&redir_node, envp, &shell);

    dup2(saved_stdin, 0);
    close(saved_stdin);

    cr_assert_stdout_eq_str("heredoc_line\n");
    free_env_list(shell.env);
}

Test(exec_redir, redir_errors, .init = redirect_all_std)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    mysh_t shell;
    char *args_cmd[] = {"echo", "test", NULL};
    char *args_file[] = {"/root/forbidden.txt", NULL};
    ast_node_t cmd_node = {NODE_COMMAND, args_cmd, NULL, NULL};
    ast_node_t file_node = {NODE_COMMAND, args_file, NULL, NULL};
    ast_node_t redir_node = {NODE_REDIR_R, NULL, &cmd_node, &file_node};

    shell.env = env_to_list(envp);
    shell.alias = NULL;

    exec_redir_node(&redir_node, envp, &shell); 

    redir_node.type = NODE_REDIR_RR;
    exec_redir_node(&redir_node, envp, &shell);

    args_file[0] = "/tmp/does_not_exist_42sh_xyz.txt";
    redir_node.type = NODE_REDIR_L; 
    exec_redir_node(&redir_node, envp, &shell);

    cr_assert(1);
    free_env_list(shell.env);
}

Test(history_nav, up_empty_history)
{
    history_t *h = history_init(10);
    line_state_t st = {0};
    char buffer[256] = {0};

    history_nav_up(buffer, &st, h);
    cr_assert_null(st.nav_cursor);
    cr_assert_str_eq(buffer, "");

    history_nav_down(buffer, &st, h);
    cr_assert_null(st.nav_cursor);

    history_free(h);
}

Test(history_nav, basic_up_and_down)
{
    history_t *h = history_init(10);
    line_state_t st = {0};
    char buffer[256] = {0};

    history_add(h, "cmd1");
    history_add(h, "cmd2");
    
    history_nav_up(buffer, &st, h);
    cr_assert_not_null(st.nav_cursor);
    cr_assert_str_eq(buffer, "cmd2");
    cr_assert_eq(st.i, 4);

    history_nav_up(buffer, &st, h);
    cr_assert_str_eq(buffer, "cmd1");

    history_nav_up(buffer, &st, h);
    cr_assert_str_eq(buffer, "cmd1");

    history_nav_down(buffer, &st, h);
    cr_assert_str_eq(buffer, "cmd2");

    history_nav_down(buffer, &st, h);
    cr_assert_null(st.nav_cursor);
    cr_assert_str_eq(buffer, "");

    history_free(h);
    if (st.saved_draft != NULL)
        free(st.saved_draft);
}

Test(history_nav, save_and_restore_draft)
{
    history_t *h = history_init(10);
    line_state_t st = {0};
    char buffer[256] = "echo un_brouillon_non_fini";

    history_add(h, "old_command");
    
    st.max_len = 26;
    st.i = 26;

    history_nav_up(buffer, &st, h);
    cr_assert_str_eq(buffer, "old_command");
    cr_assert_not_null(st.saved_draft);
    cr_assert_str_eq(st.saved_draft, "echo un_brouillon_non_fini");

    history_nav_down(buffer, &st, h);
    cr_assert_str_eq(buffer, "echo un_brouillon_non_fini");
    cr_assert_null(st.nav_cursor);

    history_free(h);
    if (st.saved_draft != NULL)
        free(st.saved_draft);
}

Test(history_nav, interactive_redraw, .init = redirect_all_std)
{
    history_t *h = history_init(10);
    line_state_t st = {0};
    char buffer[256] = {0};

    history_add(h, "ls -la");
    
    st.interactive = 1;

    history_nav_up(buffer, &st, h);

    cr_assert_stdout_eq_str("\r\033[K$> ls -la");

    history_free(h);
    if (st.saved_draft != NULL)
        free(st.saved_draft);
}