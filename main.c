#include "preprocessor.h"

int main() {
  char *input_file_name = "input/assembly.txt";
  char *output_file_name = "output/output.txt";

  preprocess(input_file_name, output_file_name);

  return 0;
}