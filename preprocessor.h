#ifndef __PREPROCESSOR__H__
#define __PREPROCESSOR__H__

/**
 * @file preprocessor.h
 * @brief This file contains the definition and manipulation functions for the
 * preprocessor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct Macro
 * @brief A structure to represent a macro in the preprocessor.
 *
 * This structure represents a macro, which is a piece of code that can be given
 * a name and then the name can be used in the program instead of the code. The
 * structure contains the name of the macro, the body of the macro (an array of
 * strings, each representing a line of code), the number of lines in the body,
 * and a pointer to the next macro in a linked list of macros.
 *
 * @param name
 * Member 'name' is a pointer to a string that represents the name of the macro.
 *
 * @param body
 * Member 'body' is a pointer to an array of strings that represent the body of
 * the macro.
 *
 * @param Macro
 * Member 'num_lines' is an integer that represents the number of lines in the
 * body of the macro.
 *
 * @param Macro
 * Member 'next' is a pointer to the next macro in a linked list of macros.
 */
typedef struct Macro {
  char *name;
  char **body;
  int num_lines;
  struct Macro *next;
} Macro;

/**
 * @file preprocessor.c
 * @brief This file contains the definitions for preprocessing an assembly file.
 */

/**
 * @brief Preprocesses an assembly file.
 *
 * @param file_name The name of the file to preprocess.
 * @return The name of the preprocessed file.
 */
char *preprocess(const char *file_name);

/**
 * @brief Gets a macro from a line buffer.
 *
 * @param line_buffer The line buffer to get the macro from.
 * @param line_number The line number for error reporting.
 * @return A pointer to the macro.
 */
Macro *get_macro(char *line, int *line_number);

/**
 * @brief Adds a macro to a macro list.
 *
 * @param macro_list The macro list to add the macro to.
 * @param new_macro The macro to add.
 */
void add_macro(Macro **macro_list, Macro *new_macro);

/**
 * @brief Finds a macro in a macro list.
 *
 * @param macro_list The macro list to find the macro in.
 * @param name The name of the macro to find.
 * @return A pointer to the macro if found, NULL otherwise.
 */
Macro *find_macro(Macro *macro_list, const char *name);

/**
 * @brief Frees a macro list.
 *
 * @param macro_list The macro list to free.
 */
void free_macro_list(Macro *macro_list);

#endif