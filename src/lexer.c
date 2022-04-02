/**
 * @file lexer.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief A lexical analyzer for the Chiton shell
 * @version 0.1
 * @date 2022-04-02
 *
 * @copyright MIT License
 *
 */

#include <stdlib.h>

#include "lexer.h"

char *token_type_str[] = {
    " ", " ", " ", " ", "\n", " ", "&&", "||", ";;", "<<", ">>", "<&", ">&", "<>", "<<-", ">|", " ",

};

/**
 * @brief FSM transitions
 */
struct branch const transitions[9][9] = {
    //       EOF                                '&'                                   '|'                                 ';'                                '<'                                   '>'                                 '-'                             letter                    digit                  States/Inputs
    {{STATE_GENERAL,     &nop}, {STATE_IN_OPERATOR_AMPERSAND,     &nop}, {STATE_IN_OPERATOR_PIPE,     &nop}, {STATE_IN_OPERATOR_SEMI,     &nop}, {STATE_IN_OPERATOR_LESS,     &nop}, {STATE_IN_OPERATOR_GREAT,     &nop}, {STATE_IN_OPERATOR_DASH,     &nop}, {STATE_IN_WORD,     &nop}, {STATE_IN_WORD,     &nop}},    // GENERAL
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND, &delimit}, {STATE_IN_OPERATOR_PIPE, &delimit}, {STATE_IN_OPERATOR_SEMI, &delimit}, {STATE_IN_OPERATOR_LESS, &delimit}, {STATE_IN_OPERATOR_GREAT, &delimit}, {STATE_IN_OPERATOR_DASH, &delimit}, {STATE_IN_WORD,     &nop}, {STATE_IN_WORD,     &nop}},    // IN_WORD
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND,     &nop}, {STATE_IN_OPERATOR_PIPE,     &nop}, {STATE_IN_OPERATOR_SEMI, &delimit}, {STATE_IN_OPERATOR_LESS, &delimit}, {STATE_IN_OPERATOR_GREAT, &delimit}, {STATE_IN_OPERATOR_DASH, &delimit}, {STATE_IN_WORD, &delimit}, {STATE_IN_WORD, &delimit}},    // IN_OPERATOR_AMPERSAND
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND, &delimit}, {STATE_IN_OPERATOR_PIPE,     &nop}, {STATE_IN_OPERATOR_SEMI, &delimit}, {STATE_IN_OPERATOR_LESS, &delimit}, {STATE_IN_OPERATOR_GREAT, &delimit}, {STATE_IN_OPERATOR_DASH, &delimit}, {STATE_IN_WORD, &delimit}, {STATE_IN_WORD, &delimit}},    // IN_OPERATOR_PIPE
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND, &delimit}, {STATE_IN_OPERATOR_PIPE, &delimit}, {STATE_IN_OPERATOR_SEMI,     &nop}, {STATE_IN_OPERATOR_LESS, &delimit}, {STATE_IN_OPERATOR_GREAT, &delimit}, {STATE_IN_OPERATOR_DASH, &delimit}, {STATE_IN_WORD, &delimit}, {STATE_IN_WORD, &delimit}},    // IN_OPERATOR_SEMI
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND,     &nop}, {STATE_IN_OPERATOR_PIPE, &delimit}, {STATE_IN_OPERATOR_SEMI, &delimit}, {STATE_IN_OPERATOR_LESS,     &nop}, {STATE_IN_OPERATOR_GREAT,     &nop}, {STATE_IN_OPERATOR_DASH,     &nop}, {STATE_IN_WORD, &delimit}, {STATE_IN_WORD, &delimit}},    // IN_OPERATOR_LESS  
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND,     &nop}, {STATE_IN_OPERATOR_PIPE,     &nop}, {STATE_IN_OPERATOR_SEMI, &delimit}, {STATE_IN_OPERATOR_LESS, &delimit}, {STATE_IN_OPERATOR_GREAT,     &nop}, {STATE_IN_OPERATOR_DASH, &delimit}, {STATE_IN_WORD, &delimit}, {STATE_IN_WORD, &delimit}},    // IN_OPERATOR_GREAT
    {{STATE_GENERAL, &delimit}, {STATE_IN_OPERATOR_AMPERSAND, &delimit}, {STATE_IN_OPERATOR_PIPE, &delimit}, {STATE_IN_OPERATOR_SEMI, &delimit}, {STATE_IN_OPERATOR_LESS, &delimit}, {STATE_IN_OPERATOR_GREAT, &delimit}, {STATE_IN_OPERATOR_DASH,     &nop}, {STATE_IN_WORD, &delimit}, {STATE_IN_WORD, &delimit}},    // IN_OPERATOR_DASH
    {{STATE_GENERAL,     &nop}, {STATE_IN_OPERATOR_AMPERSAND,     &nop}, {STATE_IN_OPERATOR_PIPE,     &nop}, {STATE_IN_OPERATOR_SEMI,     &nop}, {STATE_IN_OPERATOR_LESS,     &nop}, {STATE_IN_OPERATOR_GREAT,     &nop}, {STATE_IN_OPERATOR_DASH,     &nop}, {STATE_IN_WORD,     &nop}, {STATE_IN_WORD,     &nop}}     // EOF
};

/**
 * @brief Create a new token with the given text and type
 *
 * @param text The token's text representation
 * @param type The token type
 * @return struct token* A struct containing the token information
 */
struct token *token_create(char *text, enum token_type type)
{
    struct token *new_token = malloc(sizeof(struct token));
    new_token->text = text;
    new_token->type = type;

    return new_token;
}

/**
 * @brief Free memory used by a token
 *
 * @param token The token to free
 */
void token_free(struct token *token)
{
    if (token->text) {
        free(token->text);
    }
    free(token);
}

/**
 * @brief Creates a new list node containing a token
 *
 * @param token The token to assign to the new node
 * @return struct node* A list node containing a token
 */
struct node *node_create(struct token *token)
{
    struct node *new_node = malloc(sizeof(struct node));
    new_node->token = token;
    new_node->next = NULL;

    return new_node;
}

/**
 * @brief Free memory used by a list node
 *
 * @param node The node to free
 */
void node_free(struct node *node)
{
    if (node->token) {
        token_free(node->token);
    }
    free(node);
}

/**
 * @brief Create a new linked list for storing tokens
 *
 * @return struct token_list* A pointer to a new linked list struct
 */
struct token_list *token_list_create()
{
    struct token_list *new_list = malloc(sizeof(struct token_list));
    new_list->head = NULL;

    return new_list;
}

/**
 * @brief Delete all nodes in a token list and free any used memory
 *
 * @param list The list to free
 */
void token_list_free(struct token_list *list)
{
    struct node *current = list->head;
    struct node *next = NULL;

    while (current) {
        next = current->next;
        node_free(current);
        current = next;
    }
    list->head = NULL;

    free(list);
}

/* TODO: implement */
void delimit(struct token_list *list, const char *str) {}

void nop(struct token_list *list, const char *str) {}

/**
 * @brief Split an input string into tokens
 *
 * @param src The string to split
 * @return struct token_list* A list of tokens from the src string
 */
struct token_list *tokenize(const char *src)
{
    enum state lexer_state = STATE_GENERAL;
    while (lexer_state != STATE_EOF) {
    }

    return NULL;
}
