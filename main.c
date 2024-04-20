#include "lexer.h"
#include "parser.h"
#include "preprocessor.h"

int main() {
  char *txt_file = "input/assembly.txt";
  char *am_file = "output/output.am";
  char str[] = "MAIN: mov arr[2], r1";
  /*char str[] = "XYZ: .data 1, 10";*/
  /*char str[] = ".string \"abcdef\"";*/
  /*char str[] =
     "DFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG" "GGRES:
     .entry MAIN";*/
  Tokens tokens = {0};
  AST *root = NULL;

  /*int n = 0;
  char *binary = NULL;
  char *base4 = NULL;
  char *encoded = NULL;*/

  preprocess(txt_file, am_file);
  printf("Preprocessing completed.\n");

  tokens = split_line_to_tokens(str);
  root = parse_tokens(&tokens);

  free_ast(root);
  printf("\n");

  if (tokens.count) {
    int i;

    for (i = 0; i < tokens.count; i++) {
      printf("Token %d: %s\n", i + 1, tokens.tokens[i]);
    }
  }

  /*printf("Enter a decimal number: ");

  scanf("%d", &n);

  binary = decimalToBinary(n);
  base4 = decimalToBase4(n);
  encoded = encodeBase4(base4);

  if (binary) {
    printf("Binary representation: %s\n", binary);
    free(binary);
  }

  if (base4) {
    printf("Base 4 representation: %s\n", base4);
    free(base4);
  }

  if (encoded) {
    printf("Encoded base 4 representation: %s\n", encoded);
    free(encoded);
  }*/

  free_tokens(&tokens);

  return 0;
}