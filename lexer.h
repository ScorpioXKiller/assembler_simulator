#ifndef __LEXER__H__
#define __LEXER__H__

/**
 * @file lexer.h
 * @brief This file contains the definition and manipulation functions for the
 * lexer.
 */

#include "utils.h"
#include <ctype.h>
#include <stdbool.h>

/**
 * @struct Tokens
 * @brief A structure to represent tokens.
 *
 * This structure represents tokens, which are strings that are split from a
 * line of text. The structure contains an array of tokens and a count of the
 * number of tokens.
 *
 * @param tokens
 * Member 'tokens' is a pointer to an array of strings, each representing a
 * token.
 *
 * @param count
 * Member 'count' is an integer that represents the number of tokens.
 */
typedef struct Tokens {
  char **tokens;
  int count;
} Tokens;

/**
 * @brief Splits a line into tokens.
 *
 * This function takes a line of text and splits it into tokens based on
 * whitespace and commas. Each token is stored in a dynamically allocated array
 * of strings. The function also counts the number of tokens and stores it in
 * the Tokens structure. If memory allocation fails, the function prints an
 * error message and exits the program.
 *
 * @param line The line of text to split into tokens.
 * @return A Tokens structure containing the tokens and the count of tokens.
 */
Tokens* split_line_to_tokens(char *line);

/**
 * @brief Removes a specific token from a Tokens structure.
 *
 * This function takes a Tokens structure and a token, and removes the first
 * occurrence of the token from the Tokens structure. The function also
 * reallocates the memory for the tokens array in the Tokens structure to match
 * the new number of tokens. If memory allocation fails, the function prints an
 * error message and exits the program.
 *
 * @param t A pointer to the Tokens structure.
 * @param token_to_remove The token to remove.
 */
void remove_token(Tokens *t, const char *token_to_remove);

/**
 * @brief Frees the memory allocated for a Tokens structure.
 *
 * This function takes a pointer to a Tokens structure and frees the memory
 * allocated for the tokens array in the structure. It also sets the pointers in
 * the tokens array and the tokens array itself to NULL.
 *
 * @param tokens A pointer to the Tokens structure to free.
 */
void free_tokens(Tokens *tokens);

#endif