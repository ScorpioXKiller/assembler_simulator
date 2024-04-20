#include "utils.h"
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

char *strdup(const char *str) {
  size_t len = strlen(str);
  char *duplicate = (char *)malloc(len + 1);

  if (duplicate == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  strcpy(duplicate, str);

  return duplicate;
}