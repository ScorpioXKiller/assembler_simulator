#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Macro {
  char *name;
  char **body;
  int num_lines;
  struct Macro *next;
} Macro;

/* Function prototypes */

/* Get a macro from the input file */
Macro *get_macro(char *line);

/* Add a macro to the table */
void add_macro(Macro **macro_list, Macro *new_macro);

Macro *find_macro(Macro *macro_list, const char *name);

void print_macro_list(Macro *macro_list);

void free_macro_list(Macro *macro_list);

size_t memory_size_of_macro_list(Macro *macro_list);

void preprocess(const char *input_file_name, const char *output_file_name);