/**
 * @file symbol_table.h
 * @brief This file contains the definition and manipulation functions for the
 * symbol table.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @enum Attribute
 *  @brief Enumerates the possible attributes of a symbol.
 */
typedef enum { MDEFINE, MDATA, INTERNAL, EXTERNAL, CODE } Attribute;

/** @struct Symbol
 *  @brief Represents a symbol in the symbol table.
 */
typedef struct Symbol {
  char symbol_name[MAX_LABEL_LENGTH]; /**< The name of the symbol. */
  Attribute attribute;                /**< The attribute of the symbol. */
  int value;                          /**< The value of the symbol. */
  struct Symbol *next; /**< A pointer to the next symbol in the table. */
} Symbol;

/** @brief Looks up a symbol in the symbol table.
 *
 *  @param symbol_name The name of the symbol to look up.
 *  @param table The symbol table.
 *  @return A pointer to the symbol if found, NULL otherwise.
 */
Symbol *lookup(char symbol_name[MAX_LABEL_LENGTH], Symbol *table);

/** @brief Adds a symbol to the symbol table.
 *
 *  @param label The name of the symbol to add.
 *  @param attribute The attribute of the symbol.
 *  @param value The value of the symbol.
 *  @param new_symbol A pointer to the symbol to add.
 *  @param table A pointer to the symbol table.
 */
void add_symbol(char *label, Attribute attribute, int value, Symbol *new_symbol,
                Symbol **table);

/** @brief Frees the memory allocated for the symbol table.
 *
 *  @param table The symbol table.
 */
void free_table(Symbol *table);

#endif