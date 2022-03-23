/**
 * @file chiton.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief A simple UNIX shell
 * @version 0.1
 * @date 2021-06-30
 *
 * @copyright MIT License
 *
 */

#include <bits/posix1_lim.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error.h"

#define CHITON_TOK_BUFSIZE 64
#define CHITON_TOK_DELIM " \t\r\n\a"

/**
 * @brief Reads a line from standard input.
 *
 * @return A pointer to the line that was read.
 */
char *chiton_read_line()
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof_unlocked(stdin)) {
            /* We've received EOF. */
            exit(CHITON_ERROR_SUCCESS);
        }
        else {
            perror("readline");
            exit(CHITON_ERROR_GENERAL);
        }
    }

    return line;
}

/**
 * @brief Splits a line of text into tokens.
 *
 * @param line The line to split.
 * @return A list of tokens extracted from the line.
 */
char **chiton_tokenize(char *line)
{
    int bufsize = CHITON_TOK_BUFSIZE;
    int position = 0;
    char *token;
    char **tokens = malloc(bufsize * sizeof(char *));

    if (!tokens) {
        fprintf(stderr, "chiton: allocation error\n");
        exit(CHITON_ERROR_GENERAL);
    }

    token = strtok(line, CHITON_TOK_DELIM);
    while (token) {
        tokens[position] = token;
        ++position;

        if (position >= bufsize) {
            bufsize += CHITON_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "chiton: allocation error\n");
                exit(CHITON_ERROR_GENERAL);
            }
        }
        token = strtok(NULL, CHITON_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

/**
 * @brief Launches a new process
 *
 * @param args The arguments to pass to the new process
 * @return int The error code for the result of the process launch
 */
int launch_process(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    char *env[] = {(char *)0};

    if (pid == 0) {
        if (execve(args[0], args, env) == -1) {
            perror(args[0]);
        }
        return CHITON_ERROR_GENERAL;
    }
    else if (pid < 0) {
        perror("chiton");
        return CHITON_ERROR_GENERAL;
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return CHITON_ERROR_SUCCESS;
}

/**
 * @brief Create a prompt in the format username@hostname>
 *
 * @param buffer A malloc-allocated buffer for storing the prompt string
 * @return char* The buffer with the new prompt written to it
 */
char *create_prompt(char *buffer)
{
    char *username = malloc(sizeof(char) * _POSIX_LOGIN_NAME_MAX);
    char *hostname = malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    if (getlogin_r(username, sizeof(char) * _POSIX_LOGIN_NAME_MAX) != 0) {
        fprintf(stderr, "chiton: error fetching username\n");
        exit(CHITON_ERROR_GENERAL);
    }
    if (gethostname(hostname, sizeof(char) * _POSIX_HOST_NAME_MAX) != 0) {
        fprintf(stderr, "chiton: error fetching hostname\n");
        exit(CHITON_ERROR_GENERAL);
    }

    int prompt_len = strlen(username) + strlen(hostname) + 1;
    buffer = realloc(buffer, sizeof(char) * prompt_len);

    strcpy(buffer, username);
    strcat(buffer, "@");
    strcat(buffer, hostname);
    strcat(buffer, ">");

    free(username);
    free(hostname);

    return buffer;
}

/**
 * @brief Prints the user's prompt
 *
 * @param prompt A string containing the prompt text
 * @param status The exit code of the last command
 */
void print_prompt(const char *prompt, int status)
{
    if (status != CHITON_ERROR_SUCCESS) {
        printf("[%d] ", status);
    }
    printf("%s ", prompt);
}

int main(int argc, char **argv)
{
    char *line;
    char **args;
    int status = CHITON_ERROR_SUCCESS;
    char *prompt = NULL;

    prompt = create_prompt(prompt);
    do {
        print_prompt(prompt, status);
        line = chiton_read_line();
        args = chiton_tokenize(line);
        launch_process(args);
    } while (status != CHITON_ERROR_EXIT);

    free(line);
    free(args);
    free(prompt);

    return 0;
}