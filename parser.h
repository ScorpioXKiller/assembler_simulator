#ifndef __PARSER__H__
#define __PARSER__H__

/**
 * @file parser.h
 * @brief This file contains the definition and manipulation functions for the
 * parser.
 */

#include "consts.h"
#include "lexer.h"

/**
 * @brief Abstract Syntax Tree (AST) structure
 * It represents the parsed line of the assembly code and its type. It can be an
 * instruction, directive, comment, define, empty line or syntax error. It also
 * contains the label name, if it exists. The label name is used to reference
 * the line in the code. The AST structure contains a union of the different
 * types of lines that can be parsed.
 * INST: Represents an instruction line. It contains the instruction type and
 * its operands. DIR: Represents a directive line. It contains the directive
 * type and its operands. COMM: Represents a comment line. It contains the
 * comment string. DEFINE: Represents a define line. It contains the define name
 * and its value. EMPTY: Represents an empty line. ERROR: Represents a syntax
 * error. It contains the syntax error string.
 */
typedef struct AST {
  char *syntax_error;
  char label_name[MAX_LABEL_LENGTH];

  enum { INSTRUCTION, DIRECTIVE, COMMENT, DEFINE, ERROR, EMPTY } ASTType;

  union {
    /* Instruction */
    struct {
      enum {
        MOV,
        CMP,
        ADD,
        SUB,
        NOT,
        CLR,
        LEA,
        INC,
        DEC,
        JMP,
        BNE,
        RED,
        PRN,
        JSR,
        RTS,
        HLT
      } InstType;

      struct { /* 0 = source, 1 = destination */
        enum { /* 0 = immediate, 1 = direct, 2 = indexed, 3 = register */
               IMMEDIATE, /* immediate addressing */
               DIRECT,    /* direct addressing */
               INDEXED,   /* indexed addressing. starts with label and has two
                           */
                          /* options for index: number or label */
               REGISTER   /* register addressing */
        } OperandType;

        union {
          struct {
            enum { IMNUMBER, IMLABEL } ImmediateType;

            union {
              int number;
              char label[MAX_LABEL_LENGTH];
            } ImmediateOpt;
          } Immediate;

          char label[MAX_LABEL_LENGTH];
          int reg;

          struct {
            char label[MAX_LABEL_LENGTH];

            enum {
              INNUMBER,
              INLABEL
            } IndexType; /* choose index option between number and label */

            union {
              int number;
              char label[MAX_LABEL_LENGTH];
            } IndexOpt;
          } Index;
        } OperandOpt;
      } InstOperands[2];
    } Inst;

    /* Directive */
    struct {
      enum { DATA, STRING, ENTRY, EXTERN } DirOpt;

      union {
        struct {
          char **elements;
          int count;
        } Data;

        char *string;
        char label[MAX_LABEL_LENGTH];
      } ParamsOpt;
    } Dir;

    /* Comment */
    char *comment;

    /* Define */
    struct {
      char *name;
      int number;
    } Define;
  } ASTOpt;
} AST;

/**
 * @brief Parses tokens into an AST.
 *
 * @param tokens The tokens to parse.
 * @param line_number The line number for error reporting.
 * @param input_file_name The name of the input file for error reporting.
 * @return A pointer to the AST.
 */
AST *parse_tokens(Tokens *tokens, int line_number, const char *input_file_name);

/**
 * @brief Frees the memory allocated for an AST.
 *
 * @param ast The AST to free.
 */
void free_ast(AST *ast);

/**
 * @brief Checks if an instruction is valid.
 *
 * @param inst The instruction to check.
 * @param instIndex A pointer to an integer where the index of the instruction
 * will be stored.
 * @return true if the instruction is valid, false otherwise.
 */
bool is_instruction_valid(char *inst, int *instIndex);

/**
 * @brief Parses an instruction into an AST.
 *
 * @param inst The instruction to parse.
 * @param ast The AST to store the parsed instruction.
 */
void parse_instruction(char *inst, AST *ast);

/**
 * @brief Gets the number of operands in an AST.
 *
 * @param ast The AST to check.
 * @return The number of operands.
 */
int get_operands_count(AST *ast);

/**
 * @brief Checks if an AST has an operand.
 *
 * @param ast The AST to check.
 * @return true if the AST has an operand, false otherwise.
 */
bool is_has_operand(AST *ast);

/**
 * @brief Checks if the type of an operand in an AST is valid.
 *
 * @param ast The AST to check.
 * @param index The index of the operand to check.
 * @return true if the operand type is valid, false otherwise.
 */
bool is_operand_type_valid(AST *ast, int index);

/**
 * @brief Chooses an operand option for an AST.
 *
 * @param ast The AST to modify.
 * @param index The index of the operand to modify.
 * @param operand_type The type of the operand.
 * @param operand_value The value of the operand.
 * @param line_number The line number for error reporting.
 * @param input_file_name The name of the input file for error reporting.
 */
void choose_operand_option(AST *ast, int index, int operand_type,
                           char *operand_value, int line_number,
                           const char *input_file_name);

/**
 * @brief Identifies an operand in an AST.
 *
 * @param operand The operand to identify.
 * @param index The index of the operand.
 * @param ast The AST to modify.
 * @param line_number The line number for error reporting.
 * @param input_file_name The name of the input file for error reporting.
 * @return The type of the operand.
 */
int identify_operand(char *operand, int index, AST *ast, int line_number,
                     const char *input_file_name);

/**
 * @brief Checks if a label in an AST is valid.
 *
 * @param ast The AST to check.
 * @param label The label to check.
 * @param line_number The line number for error reporting.
 * @param input_file_name The name of the input file for error reporting.
 * @return true if the label is valid, false otherwise.
 */
bool is_label_valid(AST *ast, char *label, int line_number,
                    const char *input_file_name);

/**
 * @brief Check if a string is a digit
 *
 * @param str The string to check
 * @param max The maximum value the digit can have
 * @param min The minimum value the digit can have
 * @param result The result of the check
 * @return int 0 if the string is a digit, 1 - not a digit, -1 - out of range
 */
bool is_number_valid(char *str);

/**
 * @brief Writes an error message.
 *
 * @param ast The AST for context.
 * @param message The error message.
 * @param ... Variable arguments for the error message.
 */
void write_error_message(AST *ast, const char *message, ...);

#endif