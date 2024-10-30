#include "backend.h"
#include "errors.h"
#include "utils.h"
#include <string.h>

void rename_file(char **new_file_name, const char *old_file_name,
                 const char *extension) {
  char *extension_dot = NULL;

  *new_file_name = strdup(old_file_name);
  removeSubstring(*new_file_name, "input/");

  extension_dot = strrchr(*new_file_name, '.');

  if (extension_dot) {
    *extension_dot = '\0';
  }

  *new_file_name = STR_CAT_WITH_MALLOC(
      STR_CAT_WITH_MALLOC("output/", *new_file_name), extension);
}

void print_ob_file(Translator *translator, const char *output_file_name,
                   int *instructions, int *data) {
  char *ob_file_name = NULL;
  FILE *ob_file = NULL;
  int i = 0;

  rename_file(&ob_file_name, output_file_name, ".ob");
  ob_file = fopen(ob_file_name, "w");

  if (ob_file) {
    fprintf(ob_file, "   %d  %d\n", *instructions, *data);

    for (i = 0; i < translator->code_image->count; i++) {
      fprintf(ob_file, "%04d   %s\n", i + 100,
              translator->code_image->instructions[i]);
    }

    fclose(ob_file);
  } else {
    fprintf(stderr, ERROR_CANNOT_WRITE, ob_file_name);
  }

  free(ob_file_name);
}

void print_ent_file(Translator *translator, const char *output_file_name) {
  char *ent_file_name = NULL;
  FILE *ent_file = NULL;

  rename_file(&ent_file_name, output_file_name, ".ent");
  ent_file = fopen(ent_file_name, "w");

  if (ent_file) {
    Symbol *current_symbol = translator->internal_symbols;

    while (current_symbol) {
      if (current_symbol->attribute == INTERNAL) {
        fprintf(ent_file, "%s\t%04d\n", current_symbol->symbol_name,
                current_symbol->value);
      }
      current_symbol = current_symbol->next;
    }

    fclose(ent_file);
  } else {
    fprintf(stderr, ERROR_CANNOT_WRITE, ent_file_name);
  }
}

void print_ext_file(Translator *translator, const char *output_file_name) {
  char *ext_file_name = NULL;
  FILE *ext_file = NULL;

  rename_file(&ext_file_name, output_file_name, ".ext");
  ext_file = fopen(ext_file_name, "w");

  if (ext_file) {
    Symbol *current_symbol = translator->external_symbols;

    while (current_symbol) {
      if (current_symbol->attribute == EXTERNAL) {
        fprintf(ext_file, "%s\t\t%04d\n", current_symbol->symbol_name,
                current_symbol->value);
      }
      current_symbol = current_symbol->next;
    }

    fclose(ext_file);
  } else {
    fprintf(stderr, ERROR_CANNOT_WRITE, ext_file_name);
  }
}