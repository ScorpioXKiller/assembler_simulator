#ifndef __ASSEMBLER__H__
#define __ASSEMBLER__H__

/**
 * @file assembler.h
 * @brief This file contains the definition of the functions used in the
 * assembler.
 */

#include "translator.h"
#include "parser.h"

/**
 * @brief Perform the first pass of the assembler on the assembly file to build
 * the symbol table.
 * @param table The symbol table that will hold the symbols and their addresses
 * in the machine code.
 * @param am_file_name The name of the assembly file.
 * @param input_file_name The assembly file.
 */
bool do_first_pass(Symbol **table, FILE *am_file, const char *input_file_name);

/**
 * @brief Perform the second pass of the assembler on the assembly file to
 * generate the machine code.
 * @param translator The translator that will hold the machine code.
 * @param symbol_table The symbol table that holds the symbols and their
 * addresses in the machine code.
 * @param am_file_name The name of the assembly file.
 * @param am_file The assembly file.
 * @param instruction_counter The number of instructions in the machine code.
 * @param data_counter The number of data entries in the machine code.
 * @param input_file_name The assembly file.
 */
bool do_second_pass(Translator **translator, Symbol **symbol_table,
                    const char *am_file_name, FILE *am_file,
                    int *instruction_counter, int *data_counter,
                    const char *input_file_name);

/**
 * @brief Get the number of tokens in the current node.
 * @param current_node The current node in the AST.
 * @return The number of tokens in the current node.
 */
int get_tokens_count(AST **current_node);

/**
 * @brief Handle the instruction in the current node.
 * @param translator The translator that will hold the machine code.
 * @param current_node The current node in the AST.
 * @param symbol_table The symbol table that holds the symbols and their
 * addresses in the machine code.
 * @param instruction_counter The number of instructions in the machine code.
 * @param reg_offset The offset of the register that represents the register in
 * the machine code.
 * @param line The current line in the assembly file.
 * @param has_error A flag indicating if an error occurred.
 * @param input_file_name The assembly file.
 */
void handle_instruction(Translator *translator, AST *current_node,
                        Symbol **symbol_table, int *instruction_counter,
                        int reg_offset, int *line, bool *has_error,
                        const char *input_file_name);

/**
 * @brief Handles the directive in the current AST node during the second pass
 * of the assembler.
 *
 * This function handles the ENTRY, DATA, and STRING directives. For ENTRY, it
 * sets the attribute of the symbol to INTERNAL. For DATA, it adds the data to
 * the instructions of the code image, either by looking up the symbol or by
 * encoding the number. For STRING, it adds each character of the string to the
 * instructions of the code image.
 *
 * @param translator The translator that will hold the machine code.
 * @param current_node The current AST node being processed.
 * @param symbol_table The symbol table that holds the symbols and their
 * addresses in the machine code.
 * @param data_counter The data counter that holds the number of data entries in
 * the machine code.
 * @param line The current line number in the assembly file.
 * @param has_error Pointer to a boolean indicating if there was an error.
 */
void handle_directive(Translator *translator, AST *current_node,
                      Symbol **symbol_table, int *data_counter, int *line,
                      bool *has_erro, const char *input_file_name);

/** @brief Reallocate the instructions in the code image of the translator.
 * @param translator The translator that will hold the machine code.
 */
void reallocate_instructions(Translator *translator);

/**
 * @brief Add a symbol to the symbol table.
 * @param symbol_name The name of the symbol.
 * @param attribute The attribute of the symbol.
 * @param value The value of the symbol.
 * @param table The symbol table that holds the symbols and their addresses in
 * the machine code.
 */
void add_to_table(char symbol_name[MAX_LABEL_LENGTH], Attribute attribute,
                  int value, Symbol **table);

/**
 * @brief Encodes an immediate operand during the second pass of the assembler.
 *
 * This function handles the encoding of an immediate operand. If the operand is
 * a label, it looks up the symbol in the symbol table and encodes its value. If
 * the operand is a number, it directly encodes the number. The encoded operand
 * is then added to the instructions of the code image.
 *
 * @param translator The translator that will hold the machine code.
 * @param current_node The current AST node being processed.
 * @param symbol_table The symbol table that holds the symbols and their
 * addresses in the machine code.
 * @param operand_index The index of the operand in the instruction operands
 * array.
 * @param line The current line number in the assembly file.
 * @param has_error Pointer to a boolean indicating if there was an error.
 * @param input_file_name The assembly file.
 */
void code_immediate_operand(Translator *translator, AST *current_node,
                            Symbol **symbol_table, int operand_index, int *line,
                            bool *has_error, const char *input_file_name);

/**
 * @brief Encodes a direct operand during the second pass of the assembler.
 *
 * This function handles the encoding of a direct operand. It looks up the
 * symbol in the symbol table and encodes its value. If the symbol is internal,
 * it adds '01' to the binary representation of the symbol's value. If the
 * symbol is external, it adds '10'. If the symbol is external, it also adds the
 * symbol to the external symbols table with the current instruction counter as
 * the address. The encoded operand is then added to the instructions of the
 * code image. If the symbol is not found in the symbol table, it prints an
 * error message and sets the has_error flag to true.
 *
 * @param translator The translator that will hold the machine code.
 * @param current_node The current AST node being processed.
 * @param symbol_table The symbol table that holds the symbols and their
 * addresses in the machine code.
 * @param instruction_counter The instruction counter that holds the number of
 * instructions in the machine code.
 * @param operand_index The index of the operand in the instruction operands
 * array.
 * @param line The current line number in the assembly file.
 * @param has_error Pointer to a boolean indicating if there was an error.
 * @param input_file_name The assembly file.
 */
void code_direct_operand(Translator *translator, AST *current_node,
                         Symbol **symbol_table, int *instruction_counter,
                         int operand_index, int *line, bool *has_error,
                         const char *input_file_name);

/**
 * @brief Encodes a register operand during the second pass of the assembler.
 *
 * This function handles the encoding of a register operand. It converts the
 * register number to binary, shifts it left by the register offset, and then
 * adds the encoded register to the instructions of the code image.
 *
 * @param translator The translator that will hold the machine code.
 * @param current_node The current AST node being processed.
 * @param operand_index The index of the operand in the instruction operands
 * array.
 * @param reg_offset The offset to shift the register binary representation.
 */
void code_register_operand(Translator *translator, AST *current_node,
                           int operand_index, int reg_offset);

/**
 * @brief Encodes an indexed operand during the second pass of the assembler.
 *
 * This function handles the encoding of an indexed operand. It looks up the
 * symbol in the symbol table and encodes its value. If the index is a label, it
 * looks up the symbol for the index and encodes its value. If the index is a
 * number, it directly encodes the number. The encoded operand and index are
 * then added to the instructions of the code image. If the symbol or the index
 * symbol is not found in the symbol table, it prints an error message and sets
 * the has_error flag to true.
 *
 * @param translator The translator that will hold the machine code.
 * @param current_node The current AST node being processed.
 * @param symbol_table The symbol table that holds the symbols and their
 * addresses in the machine code.
 * @param operand_index The index of the operand in the instruction operands
 * array.
 * @param line The current line number in the assembly file.
 * @param has_error Pointer to a boolean indicating if there was an error.
 * @param input_file_name The assembly file.
 */
void code_indexed_operand(Translator *translator, AST *current_node,
                          Symbol **symbol_table, int operand_index, int *line,
                          bool *has_error, const char *input_file_name);

/**
 * @brief Encodes an instruction operand during the second pass of the
 * assembler.
 *
 * This function handles the encoding of an instruction operand based on its
 * type. It calls the appropriate function to encode the operand based on
 * whether the operand is immediate, direct, register, or indexed.
 *
 * @param translator The translator.
 * @param current_node The current AST node.
 * @param symbol_table The symbol table.
 * @param instruction_counter The instruction counter.
 * @param operand_index The index of the operand in the instruction operands
 * array.
 * @param reg_offset The offset to shift the register number by if the operand
 * is a register.
 * @param line The current line number.
 * @param has_error Pointer to a boolean indicating if there was an error.
 * @param input_file_name The assembly file.
 */
void code_inst_operand(Translator *translator, AST *current_node,
                       Symbol **symbol_table, int *instruction_counter,
                       int operand_index, int reg_offset, int *line,
                       bool *has_error, const char *input_file_name);
#endif