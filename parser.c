#include "parser.h"
#include "errors.h"
#include "lexer.h"
#include "utils.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief The instruction table. Each instruction has a name, opcode, and the
 * type of operands it accepts. The source_operand and destination_operand are
 * strings of 4 characters, each character representing a type of operand that
 * the operand supports. The types are: 0 - Immediate addressing 1 - Direct
 * addressing 2 - Indexed addressing 3 - Register addressing The first character
 * represents the first operand, the second character represents the second
 * operand. If the operand is not supported, the character is an empty string.
 * For example, the instruction "mov" supports immediate, direct, indexed, and
 * register addressing for the source operand, and direct, indexed, and register
 * addressing for the destination operand. The source_operand is "0123" and the
 * destination_operand is "123". The instruction "rts" does not support any
 * operands, so the source_operand and destination_operand are empty strings.
 * The instruction "not" does not support source operands, so the source_operand
 * is an empty string, but supports direct, indexed, and register addressing for
 * the destination operand.
 */
Instruction inst_table[INST_TABLE_SIZE] = {
    {"mov", 0, "0123", "123"}, {"cmp", 1, "0123", "0123"},
    {"add", 2, "0123", "123"}, {"sub", 3, "0123", "123"},
    {"not", 4, "", "123"},     {"clr", 5, "", "123"},
    {"lea", 6, "12", "123"},   {"inc", 7, "", "123"},
    {"dec", 8, "", "123"},     {"jmp", 9, "", "13"},
    {"bne", 10, "", "13"},     {"red", 11, "", "123"},
    {"prn", 12, "", "0123"},   {"jsr", 13, "", "13"},
    {"rts", 14, "", ""},       {"hlt", 15, "", ""}};

char *keywords[KEYWORDS_COUNT] = {
    "mov",   "cmp",     "add",    "sub",     "lea",     "not", "clr", "inc",
    "dec",   "jmp",     "bne",    "red",     "prn",     "jsr", "rts", "hlt",
    ".data", ".string", ".entry", ".extern", ".define", "r0",  "r1",  "r2",
    "r3",    "r4",      "r5",     "r6",      "r7"};

AST *parse_tokens(Tokens *tokens) {
  int tokenIndex = 0;
  int operandIndex = 0;
  char *label = NULL;
  int instIndex = 0;
  int operandType = 0;

  AST *ast = malloc(sizeof(AST));

  if (!ast) {
    return NULL;
  }

  /* Initialize all pointers to NULL */
  memset(ast, 0, sizeof(AST));

  for (tokenIndex = 0; tokenIndex < tokens->count; tokenIndex++) {
    if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
      write_error_message(ast, ERROR_UNEXPECTED_COMMA, 1, "file");

      ast->ASTType = ERROR;
      return ast;
    }

    if (tokens->tokens[tokenIndex][0] == ';') {
      ast->ASTType = COMMENT;
      return ast;
    } else if (strcmp(tokens->tokens[1], ":") == 0) {
      write_error_message(ast, ERROR_WHITESPACE_AFTER_LABEL, 1, "file");

      ast->ASTType = ERROR;
      return ast;
    } else if (tokens->tokens[tokenIndex]
                             [strlen(tokens->tokens[tokenIndex]) - 1] == ':') {
      label = strtok(tokens->tokens[tokenIndex], ":");

      if (is_label_valid(ast, label)) {
        strcpy(ast->label_name, label);
        remove_token(tokens, label);
        tokenIndex--;
      } else {
        ast->ASTType = ERROR;
        return ast;
      }
    } else if (strcmp(tokens->tokens[tokenIndex], ".define") == 0) {
      /* This is a define */
      tokenIndex++; /* Move to the next token, which should be the name */

      if (tokens->tokens[tokenIndex]) {
        if (!is_label_valid(ast, tokens->tokens[tokenIndex])) {
          ast->ASTType = ERROR;
          return ast;
        }

        ast->ASTOpt.Define.name = strdup(tokens->tokens[tokenIndex]);

        tokenIndex++; /* Move to the next token, which should be the '=' sign */

        if (tokens->tokens[tokenIndex] &&
            strcmp(tokens->tokens[tokenIndex], "=") == 0) {
          tokenIndex++; /* Move to the next token, which should be the number */

          if (tokens->tokens[tokenIndex]) {
            ast->ASTOpt.Define.number = atoi(tokens->tokens[tokenIndex]);
            ast->ASTType = DEFINE;
          } else {
            write_error_message(ast, ERROR_INVALID_DEFINE_DEFINITION,
                                ERROR_EXPECTED_NUMBER_AFTER_EQUAL_SIGN, 1,
                                "file");
            ast->ASTType = ERROR;
            return ast;
          }
        } else {
          write_error_message(ast, ERROR_INVALID_DEFINE_DEFINITION,
                              ERROR_EXPECTED_EQUAL_SIGN_AFTER_DEFINE_NAME, 1,
                              "file");

          ast->ASTType = ERROR;
          return ast;
        }
      } else {
        write_error_message(ast, ERROR_INVALID_DEFINE_DEFINITION,
                            ERROR_EXPECTED_NAME_AFTER_DEFINE, 1, "file");
        ast->ASTType = ERROR;
        return ast;
      }
    } else if (strcmp(tokens->tokens[tokenIndex], ".data") == 0) {
      /* This is a data directive */

      tokenIndex++; /* Move to the next token, which should be the data */

      if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
        write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_KEYWORD, ".data",
                            1, "file");
        return ast;
      }

      if (strcmp(tokens->tokens[tokens->count - 1], ",") == 0) {
        write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_DIRECTIVE, 1,
                            "file");
        return ast;
      }

      while (tokens->tokens[tokenIndex]) {
        if (is_number_valid(tokens->tokens[tokenIndex])) {
          if (tokens->tokens[tokenIndex + 1] &&
              strcmp(tokens->tokens[tokenIndex + 1], ",") != 0) {
            write_error_message(ast, ERROR_MISSING_COMMA_BETWEEN_NUMBERS,
                                tokens->tokens[tokenIndex],
                                tokens->tokens[tokenIndex + 1], 1, "file");
            return ast;
          }

          ast->ASTOpt.Dir.ParamsOpt.Data.numbers = (int *)realloc(
              ast->ASTOpt.Dir.ParamsOpt.Data.numbers,
              (ast->ASTOpt.Dir.ParamsOpt.Data.count + 1) * sizeof(int));
          ast->ASTOpt.Dir.ParamsOpt.Data
              .numbers[ast->ASTOpt.Dir.ParamsOpt.Data.count] =
              atoi(tokens->tokens[tokenIndex]);
          ast->ASTOpt.Dir.ParamsOpt.Data.count++;
        } else if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
          if (!tokens->tokens[tokenIndex + 1] ||
              !is_number_valid(tokens->tokens[tokenIndex + 1])) {
            write_error_message(ast, ERROR_EXTRA_COMMA_AFTER_NUMBER,
                                tokens->tokens[tokenIndex - 1], 1, "file");
            return ast;
          }
        } else {
          write_error_message(ast, ERROR_INVALID_DATA_DIRECTIVE, 1, "file");
          return ast;
        }

        tokenIndex++;
      }

      ast->ASTType = DIRECTIVE;
      ast->ASTOpt.Dir.DirOpt = DATA;

    } else if (strcmp(tokens->tokens[tokenIndex], ".string") == 0) {
      /* This is a string directive */
      tokenIndex++; /* Move to the next token, which should be the string */

      if (tokens->tokens[tokenIndex]) {
        int len = strlen(tokens->tokens[tokenIndex]);

        if (tokens->tokens[tokenIndex][0] == '"' &&
            tokens->tokens[tokenIndex][len - 1] == '"') {

          char *str_without_quotes = (char *)malloc((len - 1) * sizeof(char));

          if (str_without_quotes == NULL) {
            write_error_message(ast, ERROR_OUT_OF_MEMORY);
            ast->ASTType = ERROR;
            return ast;
          }

          strncpy(str_without_quotes, tokens->tokens[tokenIndex] + 1, len - 2);
          str_without_quotes[len - 2] = '\0';

          if (ast->ASTOpt.Dir.ParamsOpt.string != NULL) {
            free(ast->ASTOpt.Dir.ParamsOpt.string);
          }

          ast->ASTOpt.Dir.ParamsOpt.string =
              (char *)malloc((strlen(str_without_quotes) + 1) * sizeof(char));

          if (ast->ASTOpt.Dir.ParamsOpt.string == NULL) {
            write_error_message(ast, ERROR_OUT_OF_MEMORY);
            free(str_without_quotes);
            ast->ASTType = ERROR;
            return ast;
          }

          strcpy(ast->ASTOpt.Dir.ParamsOpt.string, str_without_quotes);
          free(str_without_quotes);
        } else {
          write_error_message(ast, ERROR_INVALID_STRING_DEFINITION,
                              ERROR_EXPECTED_STRING_QUOTES, 1, "file");

          ast->ASTType = ERROR;
          return ast;
        }
      } else {
        write_error_message(ast, ERROR_INVALID_STRING_DEFINITION,
                            ERROR_EXPECTED_NAME_AFTER_STRING, 1, "file");

        ast->ASTType = ERROR;
        return ast;
      }
    } else if (strcmp(tokens->tokens[tokenIndex], ".entry") == 0 ||
               strcmp(tokens->tokens[tokenIndex], ".extern") == 0) {
      /* This is an entry directive */
      tokenIndex++; /* Move to the next token, which should be the label */

      if (tokens->tokens[tokenIndex]) {
        if (is_label_valid(ast, tokens->tokens[tokenIndex])) {
          strcpy(ast->ASTOpt.Dir.ParamsOpt.label, tokens->tokens[tokenIndex]);
          ast->ASTType = DIRECTIVE;

          if (strcmp(tokens->tokens[tokenIndex - 1], ".entry") == 0) {
            ast->ASTOpt.Dir.DirOpt = ENTRY;
          } else {
            ast->ASTOpt.Dir.DirOpt = EXTERN;
          }
        } else {
          ast->ASTType = ERROR;
          return ast;
        }

      } else {
        write_error_message(ast, ERROR_EXPECTED_LABEL,
                            tokens->tokens[tokenIndex - 1], 1, "file");
        ast->ASTType = ERROR;
        return ast;
      }

    } else if (is_instruction_valid(tokens->tokens[tokenIndex], &instIndex)) {
      if (strcmp(tokens->tokens[tokens->count - 1], ",") == 0) {
        write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_INSTRUCTION,
                            tokens->tokens[tokenIndex], 1, "file");

        ast->ASTType = ERROR;
        return ast;
      }

      ast->ASTOpt.Inst.InstType = inst_table[instIndex].opcode;
      ast->ASTType = INSTRUCTION;

      if (is_has_operand(ast)) {
        int operandsCount = get_operands_count(ast);
        int i = 0;

        if (operandsCount > 1) {
          if (tokenIndex + 2 < tokens->count) {
            if (strcmp(tokens->tokens[tokenIndex + 2], ",") != 0) {
              write_error_message(ast, ERROR_EXPECTED_COMMA_AFTER_OPERAND,
                                  tokens->tokens[tokenIndex],
                                  tokens->tokens[tokenIndex + 1], 1, "file");

              ast->ASTType = ERROR;
              return ast;
            }

            if (tokenIndex + 3 < tokens->count) {
              if ((strcmp(tokens->tokens[tokenIndex + 2], ",") == 0) &&
                  (strcmp(tokens->tokens[tokenIndex + 3], ",") == 0)) {
                write_error_message(ast, ERROR_UNEXPECTED_COMMAS_AFTER_OPERAND,
                                    tokens->tokens[tokenIndex],
                                    tokens->tokens[tokenIndex + 1], 1, "file");

                ast->ASTType = ERROR;
                return ast;
              }
            }
          }
        } else if (operandsCount == 1) {
          if (tokens->count > 2 &&
              strcmp(tokens->tokens[tokenIndex + 2], ",") == 0) {
            if (tokens->count == 3) {
              write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_OPERAND,
                                  tokens->tokens[tokenIndex],
                                  tokens->tokens[tokenIndex + 1], 1, "file");
            } else if (tokens->count > 3) {
              if (strcmp(tokens->tokens[tokenIndex + 3], ",") == 0) {
                write_error_message(ast, ERROR_UNEXPECTED_COMMAS_AFTER_OPERAND,
                                    tokens->tokens[tokenIndex],
                                    tokens->tokens[tokenIndex + 1], 1, "file");
              } else {
                write_error_message(
                    ast, ERROR_UNEXPECTED_OPERANDS_AFTER_INSTRUCTION,
                    tokens->tokens[tokenIndex], operandsCount, 1, "file");
              }
            }

            ast->ASTType = ERROR;
            return ast;
          }
        }

        for (i = 0; i < tokens->count; i++) {
          remove_token(tokens, ",");
        }

        if (operandsCount > tokens->count - 1) {
          write_error_message(ast, ERROR_EXPECTED_OPERANDS_AFTER_INSTRUCTION,
                              tokens->tokens[tokenIndex], operandsCount, 1,
                              "file");

          ast->ASTType = ERROR;
          return ast;
        } else if (operandsCount < tokens->count - 1) {
          write_error_message(ast, ERROR_UNEXPECTED_OPERANDS_AFTER_INSTRUCTION,
                              tokens->tokens[tokenIndex], operandsCount, 1,
                              "file");

          ast->ASTType = ERROR;
          return ast;
        }

        for (operandIndex = 0; operandIndex < operandsCount; operandIndex++) {
          char *operand = tokens->tokens[++tokenIndex];

          if (operandsCount == 1) {
            operandIndex = 1;
          }

          operandType = identify_operand(operand, operandIndex, ast);

          if (operandType != -1) {
            if (is_operand_type_valid(ast, operandIndex)) {
              ast->ASTOpt.Inst.InstOperands[operandIndex].OperandType =
                  operandType;
              choose_operand_option(ast, operandIndex, operandType, operand);
            } else {
              write_error_message(ast, ERROR_INVALID_OPERAND_TYPE,
                                  OPERAND(operandIndex), operand,
                                  tokens->tokens[tokenIndex - 1], 1, "file");

              ast->ASTType = ERROR;
              return ast;
            }
          } else {
            write_error_message(ast, ERROR_INVALID_OPERAND,
                                tokens->tokens[tokenIndex - 1], operand, 1,
                                "file");

            ast->ASTType = ERROR;
            return ast;
          }
        }
      } else {
        if (tokens->count > 1) {
          write_error_message(ast, ERROR_UNEXPECTED_INSTRUCTION_OPERANDS,
                              tokens->tokens[tokenIndex], 1, "file");

          ast->ASTType = ERROR;
          return ast;
        }
      }
    } else {
      write_error_message(ast, ERROR_INVALID_INSTRUCTION,
                          tokens->tokens[tokenIndex], 1, "file");

      ast->ASTType = ERROR;
      return ast;
    }
  }

  return ast;
}

void write_error_message(AST *ast, const char *message, ...) {
  va_list args;
  va_start(args, message);

  if (ast->syntax_error != NULL) {
    free(ast->syntax_error);
  }

  ast->syntax_error = (char *)malloc((strlen(message) + 1) * sizeof(char));

  if (ast->syntax_error == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    va_end(args);
    return;
  }

  vsprintf(ast->syntax_error, message, args);
  va_end(args);
}

void free_ast(AST *ast) {
  int i = 0;

  if (ast == NULL) {
    return;
  }

  switch (ast->ASTType) {

  case INSTRUCTION:

    for (i = 0; i < MAX_LABEL_LENGTH; i++) {
      ast->label_name[i] = 0;
    }

    for (i = 0; i < get_operands_count(ast); i++) {
      switch (ast->ASTOpt.Inst.InstOperands[i].OperandType) {
      case IMMEDIATE:
        ast->ASTOpt.Inst.InstOperands[i].OperandOpt.number = 0;
        break;

      case DIRECT:
        free(ast->ASTOpt.Inst.InstOperands[i].OperandOpt.label);
        break;

      case INDEXED:
        switch (ast->ASTOpt.Inst.InstOperands[i].IndexType) {
        case NUMBER:
          ast->ASTOpt.Inst.InstOperands[i].OperandOpt.Index.IndexOpt.number = 0;
          break;

        case LABEL:
          free(
              ast->ASTOpt.Inst.InstOperands[i].OperandOpt.Index.IndexOpt.label);
          break;
        }

        free(ast->ASTOpt.Inst.InstOperands[i].OperandOpt.label);
        break;

      case REGISTER:
        ast->ASTOpt.Inst.InstOperands[i].OperandOpt.reg = 0;
        break;
      }
    }

    break;

  case DIRECTIVE:
    switch (ast->ASTOpt.Dir.DirOpt) {
    case DATA:
      free(ast->ASTOpt.Dir.ParamsOpt.Data.numbers);
      ast->ASTOpt.Dir.ParamsOpt.Data.count = 0;
      break;

    case STRING:
      free(ast->ASTOpt.Dir.ParamsOpt.string);
      break;

    case ENTRY:
    case EXTERN:
      for (i = 0; i < MAX_LABEL_LENGTH; i++) {
        ast->label_name[i] = 0;
      }

      break;
    }

  case DEFINE:
    free(ast->ASTOpt.Define.name);
    ast->ASTOpt.Define.number = 0;
    break;

  case COMMENT:
    free(ast->ASTOpt.comment);

  case ERROR:
    free(ast->syntax_error);
    break;
  }

  free(ast);
}

bool is_label_valid(AST *ast, char *label) {
  int i;

  if (strlen(label) > MAX_LABEL_LENGTH) {
    write_error_message(ast, ERROR_LABEL_NAME_TOO_LONG, label, MAX_LABEL_LENGTH,
                        1, "file");
    return false;
  }

  for (i = 0; i < KEYWORDS_COUNT; i++) {
    if (strcmp(label, keywords[i]) == 0) {
      write_error_message(ast, ERROR_LABEL_NAME_IS_KEYWORD, keywords[i], 1,
                          "file");
      return false;
    }
  }

  if (label) {
    if (!isalpha(label[0])) {
      write_error_message(ast, ERROR_LABEL_CANNOT_START_WITH_NUM, 1, "file");
      return false;
    }

    for (i = 1; i < strlen(label); i++) {
      if (!isalnum(label[i])) {
        write_error_message(ast, ERROR_LABEL_NAME_NOT_LETTER_OR_NUM, 1, "file");
        return false;
      }
    }
  }

  return true;
}

bool is_instruction_valid(char *inst, int *instIndex) {
  int i;

  for (i = 0; i < INST_TABLE_SIZE; i++) {
    if (strcmp(inst_table[i].name, inst) == 0) {
      *instIndex = i;
      return true;
    }
  }

  return false;
}

bool is_operand_type_valid(AST *ast, int index) {
  /* if operand type of the given instruction of the AST is match with the
  / operand type of the given instruction in the instruction table */
  int operand_type = ast->ASTOpt.Inst.InstOperands[index].OperandType;

  if (index == 0) {
    return isDigitInString(inst_table[ast->ASTOpt.Inst.InstType].source_operand,
                           operand_type);

  }

  else if (index == 1) {
    return isDigitInString(
        inst_table[ast->ASTOpt.Inst.InstType].destination_operand,
        operand_type);
  }

  return false;
}

/**
 * @brief Check if a string is a digit
 *
 * @param str The string to check
 * @param max The maximum value the digit can have
 * @param min The minimum value the digit can have
 * @param result The result of the check
 * @return int 0 if the string is a digit, 1 - not a digit, -1 - out of range
 */

bool is_number_valid(char *str) {
  int i;

  if (strlen(str) >= 1) {
    if (!isdigit(str[0]) && str[0] != '+' && str[0] != '-') { /* Check if the
                                                                first character
                                                                is a digit or a
                                                                sign */
      return false;
    }
  }

  if (strlen(str) > 1) {
    if ((str[0] == '+' || str[0] == '-') && str[1] == '0') { /* Check if the
                                                                number starts
                                                                with a sign
                                                                and a zero */
      return false;
    }

    for (i = 1; i < strlen(str); i++) { /* Check if all characters are digits */
      if (!isdigit(str[i])) {
        return false;
      }
    }
  }

  return true;
}

bool is_has_operand(AST *ast) {
  return get_operands_count(ast) > 0 ? true : false;
}

int get_operands_count(AST *ast) {
  switch (ast->ASTOpt.Inst.InstType) {
  case RTS:
  case HLT:
    return 0;
  case NOT:
  case CLR:
  case INC:
  case DEC:
  case JMP:
  case BNE:
  case RED:
  case PRN:
  case JSR:
    return 1;
  case MOV:
  case CMP:
  case ADD:
  case SUB:
  case LEA:
    return 2;
  }

  return -1;
}

int identify_operand(char *operand, int index, AST *ast) {
  char *ptr = NULL;
  char *operand_copy = strdup(operand);

  /* Remove trailing comma */
  if (operand_copy[strlen(operand_copy) - 1] == ',') {
    operand_copy[strlen(operand_copy) - 1] = '\0';
  }

  /* NULL check */
  if (!operand_copy)
    return -1;

  /* Starts with a digit = fail */
  if (isdigit(operand_copy[0]))
    return -1;

  if (IS_REGISTER(operand_copy)) {
    ast->ASTOpt.Inst.InstOperands[index].OperandType = REGISTER;

    free(operand_copy);
    return REGISTER;
  }

  ptr = operand_copy;
  /* alpha + alphanumeric[] = label */
  if (isalpha(*ptr)) {
    while (1) {
      if (isalnum(*ptr)) {
        ptr++;
      } else if (*ptr != '\0') {
        break;
      } else {
        ast->ASTOpt.Inst.InstOperands[index].OperandType = DIRECT;

        return DIRECT;
      }
    }
  }

  ptr = operand_copy + 1;
  /* '#' + number = immediate */
  if (*operand_copy == '#' && is_number_valid(ptr) && *ptr == '\0') {
    ast->ASTOpt.Inst.InstOperands[index].OperandType = IMMEDIATE;
    return IMMEDIATE;
  }

  ptr = operand_copy;
  /* alpha + alphanumeric[] + '[' + number/label + ']' = indexed */
  if (isalpha(*ptr)) {
    while (1) {
      if (isalnum(*ptr)) {
        ptr++;
      } else if (*(ptr++) == '[' && *(ptr += strlen(ptr) - 1) == ']') {
        ast->ASTOpt.Inst.InstOperands[index].OperandType = INDEXED;
        return INDEXED;
      } else {
        break;
      }
    }
  }

  /* Default = fail */
  return -1;
}

void choose_operand_option(AST *ast, int index, int operand_type,
                           char *operand_value) {
  char *ptr = NULL;
  char *open_bracket_ptr = NULL;
  char *operand_value_copy = NULL;
  int number = 0;

  switch (operand_type) {
  case IMMEDIATE:
    ast->ASTOpt.Inst.InstOperands[index].OperandOpt.number =
        atoi(operand_value);
    break;
  case DIRECT:
    ast->ASTOpt.Inst.InstOperands[index].OperandOpt.label =
        strtok(operand_value, ",");
    break;
  case INDEXED:
    operand_value_copy = strdup(operand_value);
    ptr = strchr(operand_value_copy, '[');

    if (ptr != NULL) {
      *ptr = '\0';
    }

    ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.label =
        operand_value_copy;

    /*ptr = operand_value + 1;  Skip the '[' */
    operand_value_copy = strdup(operand_value);
    open_bracket_ptr = strchr(operand_value_copy, '[');

    if ((sscanf(open_bracket_ptr, "[%d]", &number) == 1)) {
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexOpt.number =
          number;
      ast->ASTOpt.Inst.InstOperands[index].IndexType = NUMBER;
    } else {
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexOpt.label =
          open_bracket_ptr + 1;
      strtok(open_bracket_ptr, "]");
      ast->ASTOpt.Inst.InstOperands[index].IndexType = LABEL;
    }

    operand_value_copy = NULL;
    open_bracket_ptr = NULL;
    break;
  case REGISTER:
    ptr = operand_value + 1; /* Skip the 'r' */
    ptr = strtok(ptr, ",");
    ast->ASTOpt.Inst.InstOperands[index].OperandOpt.reg = atoi(ptr);
    break;
  default:
    break;
  }
}