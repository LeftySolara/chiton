/**
 * @file chiton.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief The entry point for the program.
 * @version 0.1
 * @date 2021-06-30
 *
 * @copyright MIT License
 *
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            exit(EXIT_SUCCESS);
        }
        else {
            perror("readline");
            exit(EXIT_FAILURE);
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
char **chiton_split_line(char *line)
{
    int bufsize = CHITON_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "chiton: allocation error\n");
        exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, CHITON_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int main(int argc, char **argv)
{
    char *line;

    printf("> ");
    line = chiton_read_line();

    return 0;
}