/*
** EPITECH PROJECT, 2025
** my_cd.c
** File description:
** my_cd for the minishell1
*/
#include "../../include/my.h"

static void update_env_var(mysh_t *shell, char *key, char *value)
{
    char *args[4];

    args[0] = "setenv";
    args[1] = key;
    args[2] = value;
    args[3] = NULL;
    my_setenv(shell, args);
}

static int handle_chdir_error(char *path, char *old)
{
    my_puterr(path);
    free(old);
    if (errno == ENOTDIR) {
        my_puterr(": Not a directory.\n");
        return 1;
    }
    if (errno == EACCES) {
        my_puterr(": Permission denied.\n");
        return 1;
    }
    my_puterr(": ");
    my_puterr(strerror(errno));
    my_puterr(".\n");
    return 1;
}

int change_directory(char *path, mysh_t *shell)
{
    char buffer[4096];
    char *old;

    if (getcwd(buffer, 4096) == NULL)
        return 1;
    old = my_strdup(buffer);
    if (chdir(path) != 0)
        return handle_chdir_error(path, old);
    getcwd(buffer, 4096);
    update_env_var(shell, "OLDPWD", old);
    update_env_var(shell, "PWD", buffer);
    free(old);
    return 0;
}

static int handle_cd_home(mysh_t *shell)
{
    char *home = my_getenv(shell->env, "HOME");

    if (home == NULL) {
        my_puterr("cd: No home directory.\n");
        return 1;
    }
    return change_directory(home, shell);
}

static int handle_cd_dash(mysh_t *shell)
{
    char *oldpwd = my_getenv(shell->env, "OLDPWD");

    if (oldpwd == NULL) {
        my_puterr(": No such file or directory.\n");
        return 1;
    }
    return change_directory(oldpwd, shell);
}

int my_cd(char **args, mysh_t *shell)
{
    if (args[1] != NULL && args[2] != NULL) {
        my_puterr("cd: Too many arguments.\n");
        return 1;
    }
    if (args[1] == NULL || my_strcmp(args[1], "~") == 0)
        return handle_cd_home(shell);
    if (my_strcmp(args[1], "-") == 0)
        return handle_cd_dash(shell);
    return change_directory(args[1], shell);
}
