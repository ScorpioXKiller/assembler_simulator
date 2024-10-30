#include "utils.h"
#include "errors.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

bool isDigitInString(const char *str, int digit) {
  int i;

  for (i = 0; str[i] != '\0'; ++i) {
    int currentDigit = str[i] - '0';

    if (currentDigit == digit) {
      return true;
    }
  }

  return false;
}

bool is_integer(const char *str) {
  char *endptr;
  strtol(str, &endptr, 10);
  return (*str != '\0' && *endptr == '\0');
}

char *strdup(const char *str) {
  size_t len = strlen(str);
  char *duplicate = (char *)malloc(len + 1);

  if (duplicate == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    exit(EXIT_FAILURE);
  }

  strcpy(duplicate, str);

  return duplicate;
}

void removeSubstring(char *str, const char *sub) {
  char *match = strstr(str, sub);
  size_t len = strlen(sub);
  if (match) {
    memmove(match, match + len, 1 + strlen(match + len));
  }
}