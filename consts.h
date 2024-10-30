#ifndef __CONSTS__H__
#define __CONSTS__H__

/**
 * @file consts.h
 * @brief This file contains constant definitions for the assembler.
 */

#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 31
#define INST_TABLE_SIZE 16
#define KEYWORDS_COUNT 29
#define MAX_MEMORY_SIZE 4096
#define MAX_WORD_SIZE 14
#define MAX_ERROR_LENGTH 200

#define STR_CAT_WITH_MALLOC(str1, str2)                                        \
  strcat(strcpy(malloc(strlen(str1) + strlen(str2) + 1), str1), str2)

#define OPERAND(index) ((index) == 0 ? "source" : "destination")
#define IS_REGISTER(str)                                                       \
  (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && str[2] == '\0')

/**
 * @brief A table of instructions for the assembler.
 *
 * This table contains the information for each instruction that the assembler
 * can handle. Each instruction has a name, opcode, valid source operand
 * addressing methods, and valid destination operand addressing methods. The
 * addressing methods are represented as strings of numbers, where each number
 * corresponds to an addressing method:
 * 0 - Immediate, 1 - Direct, 2 - Indexed, 3 - Register.
 * An empty string means that the instruction does not take that
 * type of operand.
 *
 * @note The size of the table is defined by the INST_TABLE_SIZE macro.
 */
typedef struct Isntruction {
  char *name;
  int opcode;
  const char *source_operand;
  const char *destination_operand;
} Instruction;

#endif