/**
 * @file lexer.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief A lexical analyzer for the Chiton shell
 * @version 0.1
 * @date 2022-04-02
 *
 * @copyright MIT License
 *
 */

#include <stdio.h>

enum state { STATE_GENERAL, STATE_IN_WORD, STATE_IN_OPERATOR, STATE_EOF };

enum token_type {
    TOKEN_TOKEN,
    TOKEN_WORD,
    TOKEN_NAME,
    TOKEN_ASSIGNMENT_WORD,
    TOKEN_NEWLINE,
    TOKEN_IO_NUMBER,
    TOKEN_AND_IF,
    TOKEN_OR_IF,
    TOKEN_DSEMI,
    TOKEN_DLESS,
    TOKEN_DGREAT,
    TOKEN_LESSAND,
    TOKEN_GREATAND,
    TOKEN_LESSGREAT,
    TOKEN_DLESSDASH,
    TOKEN_CLOBBER,
    TOKEN_RESERVED
};

enum char_type {
    CHAR_WHITESPACE = ' ',
    CHAR_AMPERSAND = '&',
    CHAR_PIPE = '|',
    CHAR_BANG = '!',
    CHAR_HASH = '#',
    CHAR_DOLLAR = '$',
    CHAR_LPAREN = '(',
    CHAR_RPAREN = ')',
    CHAR_LBRACE = '{',
    CHAR_RBRACE = '}',
    CHAR_LBRACKET = '[',
    CHAR_RBRACKET = ']',
    CHAR_TILDE = '~',
    CHAR_EOF = EOF,
};

extern char *token_type_str[];

struct token {
    char *text;
    token_type type;
};
