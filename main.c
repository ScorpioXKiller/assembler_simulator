/**
 * @file main.c
 * @brief This file contains the main function for the assembler simulator
 * program.
 * @author Dmitriy Gorodov
 * @ID 342725405
 * @version 1.0
 * @date 25.04.2024
 */

#include "assembler.h"
#include "backend.h"
#include "errors.h"
#include "preprocessor.h"

/**
 * @brief The main function for the assembler simulator program.
 *
 * @details This function reads the assembly files, preprocesses them, and then
 * performs the first and second passes of the assembler. It then prints the
 * object file, entry file, and external file for the translated assembly code.
 * @note The main function takes the assembly file names as command-line
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return The exit status of the program.
 */
int main(int argc, char **argv) {
  int i = 0;
  char *am_file_name = NULL;
  FILE *am_file = NULL;

  Translator *translator = NULL;
  Symbol *symbol_table = NULL;

  int instruction_counter = 0;
  int data_counter = 0;

  if (argc < 2) {
    fprintf(stderr, ERROR_MISSING_FILE_NAME);
  }

  for (i = 1; i < argc; i++) {
    am_file_name = preprocess(argv[i]);

    if (am_file_name) {
      am_file = fopen(am_file_name, "r");
      if (am_file) {
        if (!do_first_pass(&symbol_table, am_file, am_file_name)) {
          printf("First pass completed.\n");
          rewind(am_file);
          if (!do_second_pass(&translator, &symbol_table, am_file_name, am_file,
                              &instruction_counter, &data_counter, argv[i])) {
            printf("Second pass completed.\n");

            print_ob_file(translator, am_file_name, &instruction_counter,
                          &data_counter);
            printf("Object file created.\n");

            if (translator->internal_symbols) {
              print_ent_file(translator, am_file_name);
              printf("Entry file created.\n");
            }

            if (translator->external_symbols) {
              print_ext_file(translator, am_file_name);
              printf("External file created.\n");
            }
          }
        }

        fclose(am_file);
      }

      free(am_file_name);
    } else {
      fprintf(stderr, ERROR_CANNOT_READ, am_file_name);
    }
  }
  return 0;
}