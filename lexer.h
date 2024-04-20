#ifndef __LEXER__H__
#define __LEXER__H__

#include "utils.h"
#include <ctype.h>
#include <stdbool.h>

typedef struct Tokens {
  char **tokens;
  int count;
} Tokens;

Tokens split_line_to_tokens(char *line);

void remove_token(Tokens *t, const char *token_to_remove);

Tokens *free_tokens(Tokens *tokens);

#endif