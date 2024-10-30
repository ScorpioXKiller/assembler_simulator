#include "lexer.h"
#include "consts.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tokens *split_line_to_tokens(char *line) {
  int strings_count = 0;
  char *str = NULL;

  Tokens *tokens = (Tokens *)malloc(sizeof(Tokens));
  tokens->tokens = NULL;

  while (isspace(*line)) {
    line++;
  }

  if (*line == '\0') {
    return tokens;
  }

  if (strlen(line) > MAX_LINE_LENGTH) {
    printf(ERROR_LINE_TOO_LONG, strings_count++, MAX_LINE_LENGTH);
    return tokens;
  }

  if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
    line[strlen(line) - 1] = '\0';
  }

  str = strtok(line, " \t");

  while (str != NULL) {
    char *comma = strchr(str, ',');
    int commas_count = 0;
    int i = 0;

    if (comma) {
      for (i = 0; i < strlen(comma); i++) {
        commas_count++;
      }
      *comma = '\0';
    }

    if (str[0] != '\0') {
      tokens->tokens = (char **)realloc(tokens->tokens,
                                        (strings_count + 1) * sizeof(char *));
      if (tokens->tokens == NULL) {
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(EXIT_FAILURE);
      }

      tokens->tokens[strings_count] = strdup(str);
      strings_count++;
    }

    if (comma) {
      tokens->tokens = (char **)realloc(
          tokens->tokens, (strings_count + commas_count) * sizeof(char *));
      if (tokens->tokens == NULL) {
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(EXIT_FAILURE);
      }

      for (i = 0; i < commas_count; i++) {
        tokens->tokens[strings_count] = strdup(",");
        strings_count++;
      }

      str = strtok(NULL, " \t");
    } else {
      str = strtok(NULL, " \t");
    }
  }

  tokens->count = strings_count;

  return tokens;
}

void remove_token(Tokens *t, const char *token_to_remove) {
  int i, j;

  for (i = 0; i < t->count; i++) {
    if (strcmp((t->tokens)[i], token_to_remove) == 0) {
      char **temp = NULL;

      t->tokens[i] = NULL;

      for (j = i; j < t->count - 1; j++) {
        (t->tokens)[j] = (t->tokens)[j + 1];
      }

      (t->tokens)[t->count - 1] = NULL;
      t->count--;

      temp = realloc(t->tokens, t->count * sizeof(char *));

      if (temp == NULL) {
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(EXIT_FAILURE);
      }

      t->tokens = temp;

      break;
    }
  }
}

void free_tokens(Tokens *tokens) {
  int i = 0;

  for (i = 0; i < tokens->count; i++) {
    if (tokens->tokens[i] != NULL) {
      tokens->tokens[i] = NULL;
    }
  }

  free(tokens->tokens);
  free(tokens);
}