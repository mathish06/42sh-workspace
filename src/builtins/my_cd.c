/*
** EPITECH PROJECT, 2025
** my_cd.c
** File description:
** my_cd for the minishell1
*/
#include "../../include/my.h"

static void update_env_var(env_t **env, char *key, char *value)
{
    char *args[4];

    args[0] = "setenv";
    args[1] = key;
    args[2] = value;
    args[3] = NULL;
    my_setenv(env, args);
}

int change_directory(char *path, env_t **env)
{
    char buffer[4096];
    char *old;

    if (getcwd(buffer, 4096) == NULL)
        return 1;
    old = my_strdup(buffer);
    if (chdir(path) != 0) {
        if (errno == ENOTDIR) {
            my_putstr(path);
            write(2, ": Not a directory.\n", 19);
        } else {
            perror(path);
            free(old);
            return 1;
        }
    }
    getcwd(buffer, 4096);
    update_env_var(env, "OLDPWD", old);
    update_env_var(env, "PWD", buffer);
    free(old);
    return 0;
}

static int handle_cd_home(env_t **env)
{
    char *home = my_getenv(*env, "HOME");

    if (home == NULL) {
        my_putstr("cd: No home directory.\n");
        return 1;
    }
    return change_directory(home, env);
}

static int handle_cd_dash(env_t **env)
{
    char *oldpwd = my_getenv(*env, "OLDPWD");

    if (oldpwd == NULL) {
        my_putstr(": No such file or directory.\n");
        return 1;
    }
    return change_directory(oldpwd, env);
}

int my_cd(char **args, env_t **env)
{
    if (args[1] != NULL && args[2] != NULL) {
        my_putstr("cd: Too many arguments.\n");
        return 1;
    }
    if (args[1] == NULL || my_strcmp(args[1], "~") == 0)
        return handle_cd_home(env);
    if (my_strcmp(args[1], "-") == 0)
        return handle_cd_dash(env);
    return change_directory(args[1], env);
}
