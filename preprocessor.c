/* TODO: remove strdup and getline functions and use the ones from the standard
 * library instead.*/
#include "consts.h"
#include "errors.h"
#define _POSIX_C_SOURCE 200809L
#include "preprocessor.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

extern char *keywords[KEYWORDS_COUNT];

char *preprocess(const char *file_name) {

  char *line_buffer = NULL;
  size_t len = 0;
  ssize_t read;
  Macro *macro_list = NULL;
  int line_number = 1;

  FILE *as_file = NULL;
  FILE *am_file = NULL;

  char *as_file_name = NULL;
  char *am_file_name = NULL;

  as_file_name = STR_CAT_WITH_MALLOC(file_name, ".as");
  am_file_name = STR_CAT_WITH_MALLOC(file_name, ".am");

  as_file = fopen(as_file_name, "r");
  am_file = fopen(am_file_name, "w");

  if (!as_file || !am_file) {
    return NULL;
  }

  while ((read = getline(&line_buffer, &len, as_file)) != -1) {
    size_t leading_spaces = strspn(line_buffer, " ");

    if (strncmp(line_buffer + leading_spaces, "mcr", 3) == 0) {
      Macro *new_macro = get_macro(line_buffer, &line_number);

      while ((read = getline(&line_buffer, &len, as_file)) != -1 &&
             strstr(line_buffer, "endmcr") == NULL) {
        new_macro->body = realloc(new_macro->body,
                                  (new_macro->num_lines + 1) * sizeof(char *));
        new_macro->body[new_macro->num_lines] = strdup(line_buffer);
        new_macro->num_lines++;
      }

      add_macro(&macro_list, new_macro);
    } else {
      Macro *macro = find_macro(macro_list, line_buffer);

      if (macro) {
        int i;

        for (i = 0; i < macro->num_lines; i++) {
          fprintf(am_file, "%s", macro->body[i]);
        }

      } else {
        fprintf(am_file, "%s", line_buffer);
      }
    }

    line_number++;
  }

  free(line_buffer);
  free_macro_list(macro_list);

  fclose(am_file);
  fclose(as_file);

  free(as_file_name);

  return am_file_name;
}

Macro *get_macro(char *line_buffer, int *line_number) {
  char *name = NULL;
  Macro *new_macro = (Macro *)malloc(sizeof(Macro));
  new_macro->next = NULL;

  name = strtok(line_buffer, " ");
  name = strtok(NULL, " ");

  name[strlen(name) - 1] = '\0';

  new_macro->name = malloc(strlen(name) + 1);
  if (new_macro->name != NULL) {
    int i;

    for (i = 0; i < KEYWORDS_COUNT; i++) {
      if (strcmp(name, keywords[i]) == 0) {
        printf(ERROR_MACRO_NAME, name, *line_number, "file");
        free(new_macro->name);
        free(new_macro);
      }
    }

    strcpy(new_macro->name, name);
  } else {
    free(new_macro);
    return NULL;
  }

  new_macro->body = NULL;
  new_macro->next = NULL;
  new_macro->num_lines = 0;

  return new_macro;
}

void add_macro(Macro **macro_list, Macro *new_macro) {
  if (*macro_list == NULL) {
    *macro_list = new_macro;
  } else {
    Macro *current_macro = *macro_list;
    while (current_macro->next != NULL) {
      current_macro = current_macro->next;
    }
    current_macro->next = new_macro;
  }
}

Macro *find_macro(Macro *macro_list, const char *name) {
  char *macro_name = strdup(name);
  Macro *current_macro = macro_list;

  while (current_macro != NULL) {
    macro_name[strlen(macro_name) - 1] = '\0';

    if (strcmp(current_macro->name, macro_name + strspn(name, " ")) == 0) {
      free(macro_name);
      return current_macro;
    }
    current_macro = current_macro->next;
  }

  free(macro_name);
  return NULL;
}

void free_macro_list(Macro *macro_list) {
  Macro *current_macro = macro_list;
  Macro *next_macro = NULL;
  int i;

  while (current_macro != NULL) {
    next_macro = current_macro->next;
    free(current_macro->name);

    for (i = 0; i < current_macro->num_lines; i++) {
      free(current_macro->body[i]);
    }

    free(current_macro->body);
    free(current_macro);

    current_macro = next_macro;
  }
}