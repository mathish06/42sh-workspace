/*
** EPITECH PROJECT, 2026
** Untitled (Workspace)
** File description:
** unit_tests.c
*/
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
    my_env(env_list);
    cr_assert_stdout_eq_str("USER=loan\n");
    free_env_list(env_list);
}

Test(builtins_setenv, basic_setenv, .init = redirect_all_std)
{
    char *envp[] = {"USER=loan", NULL};
    env_t *env_list = env_to_list(envp);
    char *args[] = {"setenv", "TEST", "value", NULL};
    char *args_err[] = {"setenv", "T-EST", "value", NULL};
    char *args_many[] = {"setenv", "1", "2", "3", NULL};
    char *args_none[] = {"setenv", NULL};
    my_setenv(&env_list, args);
    cr_assert_str_eq(my_getenv(env_list, "TEST"), "value");
    my_setenv(&env_list, args_err);
    my_setenv(&env_list, args_many);
    my_setenv(&env_list, args_none);
    free_env_list(env_list);
}

Test(builtins_unsetenv, basic_unsetenv, .init = redirect_all_std)
{
    char *envp[] = {"USER=loan", "TEST=value", NULL};
    env_t *env_list = env_to_list(envp);
    char *args[] = {"unsetenv", "TEST", NULL};
    char *args_err[] = {"unsetenv", NULL};
    my_unsetenv(&env_list, args);
    cr_assert_null(my_getenv(env_list, "TEST"));
    my_unsetenv(&env_list, args_err);
    free_env_list(env_list);
}

Test(builtins_cd, basic_cd, .init = redirect_all_std)
{
    char *envp[] = {"HOME=/tmp", "OLDPWD=/tmp", NULL};
    env_t *env_list = env_to_list(envp);
    char *args_tmp[] = {"cd", "/tmp", NULL};
    char *args_home[] = {"cd", NULL};
    char *args_dash[] = {"cd", "-", NULL};
    char *args_many[] = {"cd", "1", "2", NULL};
    char *args_err[] = {"cd", "/impossible_directory_42", NULL};

    cr_assert_eq(my_cd(args_tmp, &env_list), 0);
    cr_assert_eq(my_cd(args_home, &env_list), 0);
    cr_assert_eq(my_cd(args_dash, &env_list), 0);
    cr_assert_eq(my_cd(args_many, &env_list), 1);
    cr_assert_eq(my_cd(args_err, &env_list), 1);

    free_env_list(env_list);
}

Test(parsing, extract_redirections)
{
    char line[] = "ls -l > out.txt < in.txt | grep a >> app.txt << EOF";
    command_t *cmd = create_command_list(line);

    cr_assert_not_null(cmd);
    cr_assert_str_eq(cmd->args[0], "ls");
    cr_assert_str_eq(cmd->args[1], "-l");
    cr_assert_str_eq(cmd->out_file, "out.txt");
    cr_assert_str_eq(cmd->in_file, "in.txt");
    cr_assert_eq(cmd->out_type, REDIR_RIGHT);
    cr_assert_eq(cmd->in_type, REDIR_LEFT);
    cr_assert_eq(cmd->separator, SEP_PIPE);

    cr_assert_not_null(cmd->next);
    cr_assert_str_eq(cmd->next->args[0], "grep");
    cr_assert_str_eq(cmd->next->out_file, "app.txt");
    cr_assert_str_eq(cmd->next->in_file, "EOF");
    cr_assert_eq(cmd->next->out_type, REDIR_DOUBLE_RIGHT);
    cr_assert_eq(cmd->next->in_type, REDIR_DOUBLE_LEFT);

    free_command_list(cmd);
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
    char *path = find_command("ls", env_list);
    char *abs_path = find_command("/bin/ls", env_list);
    cr_assert_not_null(path);
    free(path);
    cr_assert_str_eq(abs_path, "/bin/ls");
    free(abs_path);
    free_env_list(env_list);
}

Test(exec_command, builtins_execution, .init = redirect_all_std)
{
    char *envp[] = {"USER=test", NULL};
    env_t *env_list = env_to_list(envp);
    char line_env[] = "env";
    command_t *cmd_env = create_command_list(line_env);
    char line_setenv[] = "setenv TEST 42";
    command_t *cmd_setenv = create_command_list(line_setenv);
    exec_command(cmd_env, envp, &env_list);
    exec_command(cmd_setenv, envp, &env_list);
    cr_assert_str_eq(my_getenv(env_list, "TEST"), "42");
    free_command_list(cmd_env);
    free_command_list(cmd_setenv);
    free_env_list(env_list);
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

Test(exec_command, test_pipes_and_binaries)
{
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    env_t *env_list = env_to_list(envp);
    command_t *cmd_ls = create_command_list("ls");
    command_t *cmd_fail = create_command_list("unecommandequinexistepas");
    command_t *cmd_pipe = create_command_list("ls | grep a");
    exec_command(cmd_ls, envp, &env_list);
    free_command_list(cmd_ls);
    exec_command(cmd_fail, envp, &env_list);
    free_command_list(cmd_fail);
    exec_command(cmd_pipe, envp, &env_list);
    free_command_list(cmd_pipe);
    free_env_list(env_list);
}

Test(redirects, test_file_redirections)
{
    int saved_stdout = dup(1);
    int saved_stdin = dup(0);
    command_t cmd;
    int fd_in = open("test_in.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd_in, "contenu\n", 8);
    close(fd_in);
    cmd.out_type = REDIR_RIGHT;
    cmd.out_file = "test_out.txt";
    cmd.in_type = REDIR_LEFT;
    cmd.in_file = "test_in.txt";
    manage_redirections(&cmd);
    write(1, "succes\n", 7);
    dup2(saved_stdout, 1);
    dup2(saved_stdin, 0);
    close(saved_stdout);
    close(saved_stdin);
    cmd.out_type = REDIR_DOUBLE_RIGHT;
    cmd.out_file = "test_out_append.txt";
    cmd.in_type = REDIR_NONE;
    saved_stdout = dup(1);
    manage_redirections(&cmd);
    dup2(saved_stdout, 1);
    close(saved_stdout);
    remove("test_in.txt");
    remove("test_out.txt");
    remove("test_out_append.txt");
}

Test(redirects, test_haredoc_double_left)
{
    int saved_stdin = dup(0);
    int pipefd[2];
    command_t cmd;
    pipe(pipefd);
    write(pipefd[1], "ligne 1\nligne 2\nEOF\n", 20);
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
    cmd.out_type = REDIR_NONE;
    cmd.in_type = REDIR_DOUBLE_LEFT;
    cmd.in_file = "EOF";
    manage_redirections(&cmd);
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

Test(redirects, test_redir_left_fail, .exit_code = 1, .init = redirect_all_std)
{
    command_t cmd;

    cmd.in_type = REDIR_LEFT;
    cmd.out_type = REDIR_NONE;
    cmd.in_file = "/ce_fichier_n_existe_absolument_pas.txt";

    manage_redirections(&cmd);
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
