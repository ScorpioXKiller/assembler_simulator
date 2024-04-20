#ifndef __PARSER__H__
#define __PARSER__H__

#include "lexer.h"
#include <stdbool.h>

#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 80
#define INST_TABLE_SIZE 16
#define KEYWORDS_COUNT 29

#define OPERAND(index) ((index) == 0 ? "source" : "destination")
#define IS_REGISTER(str)                                                       \
  (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && str[2] == '\0')

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
  char label_name[MAX_LABEL_LENGTH + 1];

  enum { INSTRUCTION, DIRECTIVE, COMMENT, DEFINE, ERROR } ASTType;

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

        enum {
          NUMBER,
          LABEL
        } IndexType; /* choose index option between number and label */

        union {
          int number;
          char *label;
          int reg;
          struct {
            char *label;

            union { /* choose index option between number and label */
              int number;
              char *label;
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
          int *numbers;
          int count;
        } Data;

        char *string;
        char label[MAX_LABEL_LENGTH + 1];
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

typedef struct Isntruction {
  char *name;
  int opcode;
  const char *source_operand;
  const char *destination_operand;
} Instruction;

typedef struct Const {
  char *name;
  int value;
  struct Const *next;
} Const;

typedef struct ConstList {
  Const *head;
} ConstList;

void add_const_to_list(ConstList *list, char *name, int value);

int get_operands_count(AST *ast);

bool is_label_valid(AST *ast, char *label);

bool is_number_valid(char *str);

bool is_instruction_valid(char *inst, int *instIndex);

void parse_instruction(char *inst, AST *ast);

bool is_has_operand(AST *ast);

bool is_operand_type_valid(AST *ast, int index);

int identify_operand(char *operand, int index, AST *ast);

AST *parse_tokens(Tokens *tokens);

void free_ast(AST *ast);

void choose_operand_option(AST *ast, int index, int operand_type,
                           char *operand_value);

void write_error_message(AST *ast, const char *message, ...);

#endif