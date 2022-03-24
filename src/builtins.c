/**
 * @file builtins.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Built-in commands for the Chiton shell
 * @version 0.1
 * @date 2022-03-24
 *
 * @copyright MIT License
 *
 */
#include "builtins.h"

#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"

char *builtin_str[] = {"cd", "echo", "help", "exit"};

int (*builtin_func[])(char **) = {&chiton_cd, &chiton_echo, &chiton_help, &chiton_exit};

/**
 * @brief Changes the current working directory.
 *
 * @param args A list of arguments to the command
 * @return int Returns CHITON_ERROR_SUCCESS, or CHITON_ERROR_GENERAL on error
 */
int chiton_cd(char **args)
{
    char *dest;
    char *buf = malloc(sizeof(char) * PATH_MAX);
    char *old_pwd = getcwd(buf, PATH_MAX);

    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        dest = chiton_find_home();
    }
    else if (strcmp(args[1], "-") == 0) {
        dest = getenv("OLDPWD");
    }
    else {
        dest = args[1];
    }

    if (chdir(dest) != 0) {
        perror("cd");
        free(buf);
        return CHITON_ERROR_GENERAL;
    }

    setenv("OLDPWD", old_pwd, 1);
    free(buf);
    return CHITON_ERROR_SUCCESS;
}

/**
 * @brief Prints the user's input to stdout
 *
 * @param args The text to print
 * @return int This function always returns CHITON_ERROR_SUCCESS
 */
int chiton_echo(char **args)
{
    if (!args[1]) {
        printf("\n");
    }
    else if (args[1][0] == '$') {
        char *arg = args[1];
        char *env_var = getenv(++arg);

        if (!env_var) {
            printf("\n");
        }
        else {
            printf("%s\n", env_var);
        }
    }
    else {
        printf("%s\n", args[1]);
    }

    return CHITON_ERROR_SUCCESS;
}

/**
 * @brief Prints a short help message
 *
 * @param args Not used
 * @return int Always returns CHITON_ERROR_SUCCESS
 */
int chiton_help(char **args)
{
    printf("Julianne Adams's Chiton\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    int num_builtins = chiton_num_builtins();
    for (int i = 0; i < num_builtins; ++i) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return CHITON_ERROR_SUCCESS;
}

/**
 * @brief Tells the shell to exit
 *
 * @param args Not used
 * @return int Always returns CHITON_ERROR_EXIT
 */
int chiton_exit(char **args) { return CHITON_ERROR_EXIT; }

/**
 * @brief Gets the number of built-in commands
 *
 * @return int The number of built-in commands
 */
int chiton_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

/**
 * @brief Finds the home directory of the currently logged-in user
 *
 * @return char* The path to the user's home directory
 */
char *chiton_find_home()
{
    char *path_home = getenv("HOME");
    if (!path_home) {
        struct passwd *pw = getpwuid(geteuid());
        path_home = pw->pw_dir;
    }

    return path_home;
}
