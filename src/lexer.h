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

/**
 * @brief An input token with text and a category
 */
struct token {
    char *text;           /** The original text of the token */
    enum token_type type; /** The token type */
};

/**
 * @brief A node in a linked list containing a token.
 */
struct node {
    struct token *token; /** The token contained in this node */
    struct node *next;   /** A pointer to the next node in the list */
};

struct token_list {
    struct node *head; /** The first node in the list */
};

struct token *token_create(char *text, enum token_type type);
void token_free(struct token *token);

struct node *node_create(struct token *token);
void node_free(struct node *node);

struct token_list *token_list_create();
void token_list_free(struct token_list *list);

struct token_list *tokenize(const char *src);
