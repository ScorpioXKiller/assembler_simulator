#ifndef __TRANSLATOR__H__
#define __TRANSLATOR__H__

/**
 * @file translator.h
 * @brief This file contains the definition of the CodeImage and Translator structures used in translation.
 */

#include "symbol_table.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct CodeImage
 * @brief A structure to represent a code image.
 *
 * This structure represents a code image, which is a collection of
 * instructions. The structure contains an array of instructions and a count of
 * the number of instructions.
 *
 * @param instructions
 * Member 'instructions' is a pointer to an array of strings, each representing
 * an instruction.
 *
 * @param count
 * Member 'count' is an integer that represents the number of instructions in
 * the code image.
 */
typedef struct {
  char **instructions;
  int count;
} CodeImage;

/**
 * @struct Translator
 * @brief A structure to represent a translator.
 *
 * This structure represents a translator, which is responsible for translating
 * assembly code into machine code. The structure contains a code image, a
 * symbol table for internal symbols, and a symbol table for external symbols.
 *
 * @param code_image
 * Member 'code_image' is a pointer to a CodeImage structure that represents the
 * code image.
 *
 * @param internal_symbols
 * Member 'internal_symbols' is a pointer to a Symbol structure that represents
 * the symbol table for internal symbols.
 *
 * @param external_symbols
 * Member 'external_symbols' is a pointer to a Symbol structure that represents
 * the symbol table for external symbols.
 */
typedef struct {
  CodeImage *code_image;
  Symbol *internal_symbols;
  Symbol *external_symbols;
} Translator;

#endif