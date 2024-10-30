#ifndef __BACKEND__H__
#define __BACKEND__H__

/**
 * @file backend.h
 * @brief This file contains the definition and manipulation functions for the
 * backend.
 */

#include "translator.h"

/**
 * @brief Renames a file by changing its directory and adding an extension.
 *
 * This function takes the old file name, removes the "input/" substring, and
 * then prepends "output/" and appends the specified extension. The new file
 * name is allocated on the heap, so it should be freed when no longer needed.
 *
 * @param new_file_name Pointer to a string where the new file name will be
 * stored.
 * @param old_file_name The original file name.
 * @param extension The extension to be added to the new file name.
 */
void rename_file(char **new_file_name, const char *old_file_name,
                 const char *extension);

/**
 * @brief Prints the object file (.ob) for the translated assembly code.
 *
 * This function creates an object file with the specified output file name and
 * extension ".ob". It then writes the number of instructions and data to the
 * file, followed by the instructions from the code image. Each instruction is
 * printed on a new line with its address (starting from 100) and its binary
 * representation. If the file cannot be opened, the function does nothing.
 *
 * @param translator The translator that holds the machine code.
 * @param output_file_name The name of the output file.
 * @param instructions Pointer to the number of instructions.
 * @param data Pointer to the number of data.
 */
void print_ob_file(Translator *translator, const char *output_file_name,
                   int *instructions, int *data);

/**
 * @brief Prints the entry file (.ent) for the translated assembly code.
 *
 * This function creates an entry file with the specified output file name and
 * extension ".ent". It then writes the symbol name and its value for each
 * internal symbol in the symbol table to the file. Each symbol is printed on a
 * new line with its name and its address. If the file cannot be opened, the
 * function does nothing.
 *
 * @param translator The translator that holds the machine code.
 * @param output_file_name The name of the output file.
 */
void print_ent_file(Translator *translator, const char *output_file_name);

/**
 * @brief Prints the external file (.ext) for the translated assembly code.
 *
 * This function creates an external file with the specified output file name
 * and extension ".ext". It then writes the symbol name and its value for each
 * external symbol in the symbol table to the file. Each symbol is printed on a
 * new line with its name and its address. If the file cannot be opened, the
 * function does nothing.
 *
 * @param translator The translator that holds the machine code.
 * @param output_file_name The name of the output file.
 */
void print_ext_file(Translator *translator, const char *output_file_name);

#endif