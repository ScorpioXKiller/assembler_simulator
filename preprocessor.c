/* TODO: remove strdup and getline functions and use the ones from the standard
 * library instead.*/
#define _POSIX_C_SOURCE 200809L
#include "preprocessor.h"
#include <stdlib.h>
#include <string.h>

void preprocess(const char *input_file_name, const char *output_file_name) {
  char *line_buffer = NULL;
  size_t len = 0;
  ssize_t read;
  Macro *macro_list = NULL;

  FILE *input_file = fopen(input_file_name, "r");
  FILE *output_file = fopen(output_file_name, "w");

  while ((read = getline(&line_buffer, &len, input_file)) != -1) {
    size_t leading_spaces = strspn(line_buffer, " ");

    if (strncmp(line_buffer + leading_spaces, "mcr", 3) == 0) {
      Macro *new_macro = get_macro(line_buffer);

      while ((read = getline(&line_buffer, &len, input_file)) != -1 &&
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
          fprintf(output_file, "%s", macro->body[i]);
        }

      } else {
        char *trimmed_line = line_buffer + strspn(line_buffer, " ");

        if (*trimmed_line != '\0' && *trimmed_line != '\n') {
          fprintf(output_file, "%s", line_buffer);
        }
      }
    }
  }

  free(line_buffer);
  free_macro_list(macro_list);
}

Macro *get_macro(char *line_buffer) {
  char *name = NULL;
  Macro *new_macro = (Macro *)malloc(sizeof(Macro));
  new_macro->next = NULL;

  name = strtok(line_buffer, " ");
  name = strtok(NULL, " ");

  new_macro->name = malloc(strlen(name) + 1);
  if (new_macro->name != NULL) {
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
    if (strcmp(current_macro->name, strtok(macro_name, " ")) == 0) {
      free(macro_name);
      return current_macro;
    }
    current_macro = current_macro->next;
  }

  free(macro_name);
  return NULL;
}

void print_macro_list(Macro *macro_list) {
  Macro *current_macro = macro_list;
  int i;
  while (current_macro != NULL) {
    printf("Macro name: %s\n", current_macro->name);
    for (i = 0; i < current_macro->num_lines; i++) {
      printf("%s", current_macro->body[i]);
    }
    current_macro = current_macro->next;
  }
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

size_t memory_size_of_macro_list(Macro *macro_list) {
  size_t size = 0;
  Macro *current_macro = macro_list;
  int i;

  while (current_macro != NULL) {
    size += sizeof(Macro);
    size += strlen(current_macro->name) + 1;

    for (i = 0; i < current_macro->num_lines; i++) {
      size += strlen(current_macro->body[i]) + 1;
    }

    size += current_macro->num_lines * sizeof(char *);

    current_macro = current_macro->next;
  }

  return size;
}

/*char *strdup(const char *s) {
  char *d = malloc(strlen(s) + 1);
  if (d != NULL) {
    strcpy(d, s);
  }
  return d;
}*/
