#include "converter.h"
#include "errors.h"
#include <string.h>

char *to_binary14(int num) {
  char *binary14 = (char *)malloc(15);
  int i = 0;

  if (binary14 == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    exit(EXIT_FAILURE);
  }

  for (i = 13; i >= 0; i--) {
    int bit = (num >> i) & 1;
    binary14[13 - i] = bit + '0';
  }
  binary14[14] = '\0';

  return binary14;
}

char *shift_left(const char *binary_14, int offset) {
  int i = 0;
  int len = strlen(binary_14);
  char *shifted_str = (char *)malloc(len + 1);

  if (shifted_str == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < len; ++i) {
    int newIndex = (i + offset) % len;
    shifted_str[i] = binary_14[newIndex];
  }

  shifted_str[len] = '\0';

  return shifted_str;
}

char *add_binary_numbers(const char *binary14_1, const char *binary14_2) {
  int num1 = strtol(binary14_1, NULL, 2);
  int num2 = strtol(binary14_2, NULL, 2);
  int sum = num1 + num2;

  return to_binary14(sum);
}

char *to_base4_encrypted(char *binary14) {
  int len = strlen(binary14) / 2;
  char *encoded = (char *)malloc((len + 1) * sizeof(char));
  int i = 0;

  if (!encoded) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < len; i++) {
    int base4_digit = 2 * (binary14[2 * i] - '0') + (binary14[2 * i + 1] - '0');

    switch (base4_digit) {
    case 0:
      encoded[i] = '*';
      break;
    case 1:
      encoded[i] = '#';
      break;
    case 2:
      encoded[i] = '%';
      break;
    case 3:
      encoded[i] = '!';
      break;
    }
  }

  encoded[len] = '\0';

  return encoded;
}