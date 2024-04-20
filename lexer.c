#include "lexer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tokens split_line_to_tokens(char *line) {
  int strings_count = 0;
  char *s = NULL;

  Tokens tokens = {0};

  while (isspace(*line)) {
    line++;
  }

  if (*line == '\0') {
    return tokens;
  }

  s = strtok(line, " \t");

  while (s) {
    if (strrchr(s, ',') != NULL) {
      int i = 0;
      char temp = s[i];

      while (s[i] != '\0') {
        if (s[i] == ',') {
          tokens.tokens = (char **)realloc(tokens.tokens, (strings_count + 1) *
                                                              sizeof(char *));

          if (tokens.tokens == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
          }

          tokens.tokens[strings_count] = strdup(",");
          strings_count++;
          i++;
        } else {
          int start = i;

          while (s[i] != ',' && s[i] != '\0') {
            i++;
          }

          temp = s[i];
          s[i] = '\0';
          tokens.tokens = (char **)realloc(tokens.tokens, (strings_count + 1) *
                                                              sizeof(char *));
          tokens.tokens[strings_count] = strdup(&s[start]);
          strings_count++;
          s[i] = temp;
        }
      }
    } else {
      tokens.tokens =
          (char **)realloc(tokens.tokens, (strings_count + 1) * sizeof(char *));

      if (tokens.tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
      }

      tokens.tokens[strings_count] = strdup(s);
      strings_count++;
    }

    s = strtok(NULL, " \t");
  }

  tokens.count = strings_count;

  return tokens;
}

void remove_token(Tokens *t, const char *token_to_remove) {
  int i, j;

  for (i = 0; i < t->count; i++) {
    if (strcmp((t->tokens)[i], token_to_remove) == 0) {
      free((t->tokens)[i]);

      for (j = i; j < t->count - 1; j++) {
        (t->tokens)[j] = (t->tokens)[j + 1];
      }

      (t->tokens)[t->count - 1] = NULL;
      t->count--;

      t->tokens = realloc(t->tokens, t->count * sizeof(char *));

      if (t->tokens == NULL) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
      }

      break;
    }
  }
}

Tokens *free_tokens(Tokens *tokens) {
  int i = 0;

  for (i = 0; i < tokens->count; i++) {
    free(tokens->tokens[i]);
  }

  free(tokens->tokens);
  tokens->count = 0;

  return tokens;
}