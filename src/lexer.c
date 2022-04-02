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

/**
 * @brief Split an input string into tokens
 *
 * @param src The string to split
 * @return struct token_list* A list of tokens from the src string
 */
struct token_list *tokenize(const char *src)
{
    /*TODO: Implement this */
    return NULL;
}
