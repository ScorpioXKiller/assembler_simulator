#include "parser.h"
#include "errors.h"
#include "lexer.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Instruction inst_table[INST_TABLE_SIZE];
extern char *keywords[KEYWORDS_COUNT];

AST *parse_tokens(Tokens *tokens, int line_number,
                  const char *input_file_name) {
  int tokenIndex = 0;
  int operandIndex = 0;
  char *label = NULL;
  int instIndex = 0;
  int operandType = 0;

  AST *ast = malloc(sizeof(AST));

  if (!ast) {
    return NULL;
  }

  memset(ast, 0, sizeof(AST));

  if (tokenIndex == 0) {
    ast->ASTType = EMPTY;
  }

  if (tokens == NULL || tokens->tokens == NULL) {
    ast->ASTType = EMPTY;
    return ast;
  }

  for (tokenIndex = 0; tokenIndex < tokens->count; tokenIndex++) {
    if (tokens->tokens[tokenIndex] != NULL) {

      if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
        write_error_message(ast, ERROR_UNEXPECTED_COMMA, line_number,
                            input_file_name);

        ast->ASTType = ERROR;
        return ast;
      }

      if (tokens->tokens[tokenIndex][0] == ';') {
        ast->ASTType = COMMENT;
        return ast;
      } else if (tokens->count > 1 && strcmp(tokens->tokens[1], ":") == 0) {
        write_error_message(ast, ERROR_WHITESPACE_AFTER_LABEL, line_number,
                            input_file_name);

        ast->ASTType = ERROR;
        return ast;
      } else if (tokens->tokens[tokenIndex][strlen(tokens->tokens[tokenIndex]) -
                                            1] == ':') {
        label = strtok(tokens->tokens[tokenIndex], ":");

        if (is_label_valid(ast, label, line_number, input_file_name)) {
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
          if (!is_label_valid(ast, tokens->tokens[tokenIndex], line_number,
                              input_file_name)) {
            ast->ASTType = ERROR;
            return ast;
          }

          ast->ASTOpt.Define.name = strdup(tokens->tokens[tokenIndex]);

          tokenIndex++; /* Move to the next token, which should be the '=' sign
                         */

          if (tokens->tokens[tokenIndex] &&
              strcmp(tokens->tokens[tokenIndex], "=") == 0) {
            tokenIndex++; /* Move to the next token, which should be the number
                           */

            if (tokens->tokens[tokenIndex]) {
              ast->ASTOpt.Define.number = atoi(tokens->tokens[tokenIndex]);
              ast->ASTType = DEFINE;
            } else {
              write_error_message(ast, ERROR_INVALID_DEFINE_DEFINITION,
                                  ERROR_EXPECTED_NUMBER_AFTER_EQUAL_SIGN,
                                  line_number, input_file_name);
              ast->ASTType = ERROR;
              return ast;
            }
          } else {
            write_error_message(ast, ERROR_INVALID_DEFINE_DEFINITION,
                                ERROR_EXPECTED_EQUAL_SIGN_AFTER_DEFINE_NAME,
                                line_number, input_file_name);

            ast->ASTType = ERROR;
            return ast;
          }
        } else {
          write_error_message(ast, ERROR_INVALID_DEFINE_DEFINITION,
                              ERROR_EXPECTED_NAME_AFTER_DEFINE, line_number,
                              input_file_name);
          ast->ASTType = ERROR;
          return ast;
        }
      } else if (strcmp(tokens->tokens[tokenIndex], ".data") == 0) {
        /* This is a data directive */

        tokenIndex++; /* Move to the next token, which should be the data */

        if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
          write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_KEYWORD,
                              tokens->tokens[tokenIndex - 1], line_number,
                              input_file_name);
          ast->ASTType = ERROR;
          return ast;
        }

        if (strcmp(tokens->tokens[tokens->count - 1], ",") == 0) {
          write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_DIRECTIVE,
                              line_number, input_file_name);
          ast->ASTType = ERROR;
          return ast;
        }

        while (tokenIndex < tokens->count) {
          if ((strcmp(tokens->tokens[tokenIndex], ",") != 0) &&
              (is_number_valid(tokens->tokens[tokenIndex]) ||
               is_label_valid(ast, tokens->tokens[tokenIndex], line_number,
                              input_file_name))) {
            char **temp = NULL;
            if (tokenIndex + 1 < tokens->count &&
                tokens->tokens[tokenIndex + 1] &&
                strcmp(tokens->tokens[tokenIndex + 1], ",") != 0) {
              write_error_message(ast, ERROR_MISSING_COMMA_BETWEEN_NUMBERS,
                                  tokens->tokens[tokenIndex],
                                  tokens->tokens[tokenIndex + 1], line_number,
                                  input_file_name);
              ast->ASTType = ERROR;
              return ast;
            }

            temp = (char **)realloc(ast->ASTOpt.Dir.ParamsOpt.Data.elements,
                                    (ast->ASTOpt.Dir.ParamsOpt.Data.count + 1) *
                                        sizeof(char *));

            if (temp == NULL) {
              fprintf(stderr, ERROR_OUT_OF_MEMORY);
              exit(EXIT_FAILURE);
            }
            ast->ASTOpt.Dir.ParamsOpt.Data.elements = temp;

            ast->ASTOpt.Dir.ParamsOpt.Data
                .elements[ast->ASTOpt.Dir.ParamsOpt.Data.count] =
                tokens->tokens[tokenIndex];
            ast->ASTOpt.Dir.ParamsOpt.Data.count++;
          } else if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
            if (!tokens->tokens[tokenIndex + 1] ||
                strcmp(tokens->tokens[tokenIndex + 1], ",") == 0) {
              write_error_message(ast, ERROR_EXTRA_COMMA_AFTER_NUMBER,
                                  tokens->tokens[tokenIndex - 1], line_number,
                                  input_file_name);
              ast->ASTType = ERROR;
              return ast;
            }
          } else {
            write_error_message(ast, ERROR_INVALID_DATA_ELEMENT,
                                tokens->tokens[tokenIndex], line_number,
                                input_file_name);
            ast->ASTType = ERROR;
            return ast;
          }

          tokenIndex++;
        }

        ast->ASTType = DIRECTIVE;
        ast->ASTOpt.Dir.DirOpt = DATA;

        return ast;

      } else if (strcmp(tokens->tokens[tokenIndex], ".string") == 0) {
        /* This is a string directive */
        tokenIndex++; /* Move to the next token, which should be the string */

        if (tokens->tokens[tokenIndex]) {
          int len = strlen(tokens->tokens[tokenIndex]);

          if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
            write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_DIRECTIVE,
                                tokens->tokens[tokenIndex - 1], line_number,
                                input_file_name);
            ast->ASTType = ERROR;
            return ast;
          }

          if (tokens->tokens[tokenIndex][0] == '"' &&
              tokens->tokens[tokenIndex][len - 1] == '"') {

            char *str_without_quotes = (char *)malloc((len - 1) * sizeof(char));

            if (str_without_quotes == NULL) {
              fprintf(stderr, ERROR_OUT_OF_MEMORY);
              exit(EXIT_FAILURE);
            }

            strncpy(str_without_quotes, tokens->tokens[tokenIndex] + 1,
                    len - 2);
            str_without_quotes[len - 2] = '\0';

            if (ast->ASTOpt.Dir.ParamsOpt.string != NULL) {
              free(ast->ASTOpt.Dir.ParamsOpt.string);
            }

            ast->ASTOpt.Dir.ParamsOpt.string =
                (char *)malloc((strlen(str_without_quotes) + 1) * sizeof(char));

            if (ast->ASTOpt.Dir.ParamsOpt.string == NULL) {
              fprintf(stderr, ERROR_OUT_OF_MEMORY);
              exit(EXIT_FAILURE);
            }

            strcpy(ast->ASTOpt.Dir.ParamsOpt.string, str_without_quotes);
            free(str_without_quotes);
          } else {
            write_error_message(ast, ERROR_INVALID_STRING_DEFINITION,
                                ERROR_EXPECTED_STRING_QUOTES, line_number,
                                input_file_name);

            ast->ASTType = ERROR;
            return ast;
          }
        } else {
          write_error_message(ast, ERROR_INVALID_STRING_DEFINITION,
                              ERROR_EXPECTED_NAME_AFTER_STRING, line_number,
                              input_file_name);

          ast->ASTType = ERROR;
          return ast;
        }

        ast->ASTType = DIRECTIVE;
        ast->ASTOpt.Dir.DirOpt = STRING;

        return ast;
      } else if (strcmp(tokens->tokens[tokenIndex], ".entry") == 0 ||
                 strcmp(tokens->tokens[tokenIndex], ".extern") == 0) {
        int i = 0;
        if (ast->label_name[0] != '\0') {
          write_error_message(ast, WARN_LABEL_IGNORED, ast->label_name,
                              line_number, input_file_name);
          printf("%s", ast->syntax_error);

          for (i = 0; i < MAX_LABEL_LENGTH; i++) {
            ast->label_name[i] = 0;
          }
        }
        /* This is an entry directive */
        tokenIndex++; /* Move to the next token, which should be the label */

        if (tokens->tokens[tokenIndex]) {
          if (strcmp(tokens->tokens[tokenIndex], ",") == 0) {
            write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_KEYWORD,
                                tokens->tokens[tokenIndex - 1], line_number,
                                input_file_name);
            ast->ASTType = ERROR;
            return ast;
          }

          if (is_label_valid(ast, tokens->tokens[tokenIndex], line_number,
                             input_file_name)) {
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
                              tokens->tokens[tokenIndex - 1], line_number,
                              input_file_name);
          ast->ASTType = ERROR;
          return ast;
        }
      } else if (is_instruction_valid(tokens->tokens[tokenIndex], &instIndex)) {
        if (strcmp(tokens->tokens[tokens->count - 1], ",") == 0) {
          write_error_message(ast, ERROR_UNEXPECTED_COMMA_AFTER_INSTRUCTION,
                              tokens->tokens[tokenIndex], line_number,
                              input_file_name);

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
                                    tokens->tokens[tokenIndex + 1], line_number,
                                    input_file_name);

                ast->ASTType = ERROR;
                return ast;
              }

              if (tokenIndex + 3 < tokens->count) {
                if ((strcmp(tokens->tokens[tokenIndex + 2], ",") == 0) &&
                    (strcmp(tokens->tokens[tokenIndex + 3], ",") == 0)) {
                  write_error_message(ast,
                                      ERROR_UNEXPECTED_COMMAS_AFTER_OPERAND,
                                      tokens->tokens[tokenIndex],
                                      tokens->tokens[tokenIndex + 1],
                                      line_number, input_file_name);

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
                                    tokens->tokens[tokenIndex + 1], line_number,
                                    input_file_name);
              } else if (tokens->count > 3) {
                if (strcmp(tokens->tokens[tokenIndex + 3], ",") == 0) {
                  write_error_message(ast,
                                      ERROR_UNEXPECTED_COMMAS_AFTER_OPERAND,
                                      tokens->tokens[tokenIndex],
                                      tokens->tokens[tokenIndex + 1],
                                      line_number, input_file_name);
                } else {
                  write_error_message(
                      ast, ERROR_UNEXPECTED_OPERANDS_AFTER_INSTRUCTION,
                      tokens->tokens[tokenIndex], operandsCount, line_number,
                      input_file_name);
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
                                tokens->tokens[tokenIndex], operandsCount,
                                line_number, input_file_name);

            ast->ASTType = ERROR;
            return ast;
          } else if (operandsCount < tokens->count - 1) {
            write_error_message(ast,
                                ERROR_UNEXPECTED_OPERANDS_AFTER_INSTRUCTION,
                                tokens->tokens[tokenIndex], operandsCount,
                                line_number, input_file_name);

            ast->ASTType = ERROR;
            return ast;
          }

          for (operandIndex = 0; operandIndex < operandsCount; operandIndex++) {
            char *operand = tokens->tokens[++tokenIndex];

            if (operandsCount == 1) {
              operandIndex = 1;
            }

            operandType = identify_operand(operand, operandIndex, ast,
                                           line_number, input_file_name);

            if (operandType != -1) {
              if (operand[0] == '#') {
                operand = strtok(operand, "#");
              }

              if (is_operand_type_valid(ast, operandIndex)) {
                ast->ASTOpt.Inst.InstOperands[operandIndex].OperandType =
                    operandType;
                choose_operand_option(ast, operandIndex, operandType, operand,
                                      line_number, input_file_name);
              } else {
                write_error_message(ast, ERROR_INVALID_OPERAND_TYPE,
                                    OPERAND(operandIndex), operand,
                                    tokens->tokens[tokenIndex - 1], line_number,
                                    input_file_name);

                ast->ASTType = ERROR;
                return ast;
              }
            } else {
              write_error_message(ast, ERROR_INVALID_OPERAND, tokens->tokens[0],
                                  operand, line_number, input_file_name);

              ast->ASTType = ERROR;
              return ast;
            }
          }
        } else {
          if (tokens->count > 1) {
            write_error_message(ast, ERROR_UNEXPECTED_INSTRUCTION_OPERANDS,
                                tokens->tokens[tokenIndex], line_number,
                                input_file_name);

            ast->ASTType = ERROR;
            return ast;
          }
        }
      } else {
        write_error_message(ast, ERROR_INVALID_INSTRUCTION,
                            tokens->tokens[tokenIndex], line_number,
                            input_file_name);

        ast->ASTType = ERROR;
        return ast;
      }
    }
  }
  return ast;
}

void write_error_message(AST *ast, const char *message, ...) {
  va_list args;
  va_start(args, message);

  if (ast->syntax_error != NULL) {
    free(ast->syntax_error);
    ast->syntax_error = NULL;
  }

  ast->syntax_error = (char *)malloc(MAX_ERROR_LENGTH * sizeof(char));

  if (ast->syntax_error == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    va_end(args);
    exit(EXIT_FAILURE);
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

  if (ast->syntax_error != NULL) {
    if (strcmp(ast->syntax_error, "") != 0) {
      free(ast->syntax_error);
      ast->syntax_error = NULL;
    }
  }

  if (ast->ASTType == DIRECTIVE) {
    if (ast->ASTOpt.Dir.DirOpt == DATA) {
      for (i = 0; i < ast->ASTOpt.Dir.ParamsOpt.Data.count; i++) {
        free(ast->ASTOpt.Dir.ParamsOpt.Data.elements[i]);
        ast->ASTOpt.Dir.ParamsOpt.Data.elements[i] = NULL;
      }

      ast->ASTOpt.Dir.ParamsOpt.Data.count = 0;
      free(ast->ASTOpt.Dir.ParamsOpt.Data.elements);
      ast->ASTOpt.Dir.ParamsOpt.Data.elements = NULL;
    } else if (ast->ASTOpt.Dir.DirOpt == STRING) {
      free(ast->ASTOpt.Dir.ParamsOpt.string);
      ast->ASTOpt.Dir.ParamsOpt.string = NULL;
    }
  }

  if (ast->ASTType == COMMENT) {
    free(ast->ASTOpt.comment);
    ast->ASTOpt.comment = NULL;
  }

  if (ast->ASTType == DEFINE) {
    free(ast->ASTOpt.Define.name);
    ast->ASTOpt.Define.name = NULL;
  }

  ast = NULL;
}

bool is_label_valid(AST *ast, char *label, int line_number,
                    const char *input_file_name) {
  int i;

  if (strlen(label) > MAX_LABEL_LENGTH) {
    write_error_message(ast, ERROR_LABEL_NAME_TOO_LONG, label, MAX_LABEL_LENGTH,
                        line_number, input_file_name);
    return false;
  }

  for (i = 0; i < KEYWORDS_COUNT; i++) {
    if (strcmp(label, keywords[i]) == 0) {
      write_error_message(ast, ERROR_LABEL_NAME_IS_KEYWORD, keywords[i],
                          line_number, input_file_name);
      return false;
    }
  }

  if (label) {
    if (!isalpha(label[0])) {
      write_error_message(ast, ERROR_LABEL_CANNOT_START_WITH_NUM, line_number,
                          input_file_name);
      return false;
    }

    for (i = 1; i < strlen(label); i++) {
      if (!isalnum(label[i])) {
        write_error_message(ast, ERROR_LABEL_NAME_NOT_LETTER_OR_NUM,
                            line_number, input_file_name);
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
  /* Check if operand type of the given instruction of the AST is match with the
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

bool is_number_valid(char *str) {
  int i;

  if (strlen(str) >= 1) {
    if (!isdigit(str[0]) && str[0] != '+' && str[0] != '-') {
      return false;
    }
  }

  if (strlen(str) > 1) {
    if ((str[0] == '+' || str[0] == '-') && str[1] == '0') {
      return false;
    }

    for (i = 1; i < strlen(str); i++) {
      if (!isdigit(str[i])) {
        if (str[i] == '.') {
          return false;
        }
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

int identify_operand(char *operand, int index, AST *ast, int line_number,
                     const char *input_file_name) {
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
  if (operand_copy[0] == '#' &&
      (is_number_valid(ptr) ||
       is_label_valid(ast, ptr, line_number, input_file_name))) {
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
                           char *operand_value, int line_number,
                           const char *input_file_name) {
  char *ptr = NULL;
  char *open_bracket_ptr = NULL;
  char *operand_value_copy = NULL;
  int number = 0;

  switch (operand_type) {
  case IMMEDIATE:
    if (is_number_valid(operand_value)) {
      ast->ASTOpt.Inst.InstOperands[index]
          .OperandOpt.Immediate.ImmediateOpt.number = atoi(operand_value);
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Immediate.ImmediateType =
          IMNUMBER;
    } else if (is_label_valid(ast, operand_value, line_number,
                              input_file_name)) {
      strcpy(ast->ASTOpt.Inst.InstOperands[index]
                 .OperandOpt.Immediate.ImmediateOpt.label,
             operand_value);
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Immediate.ImmediateType =
          IMLABEL;
    }

    break;
  case DIRECT:
    strcpy(ast->ASTOpt.Inst.InstOperands[index].OperandOpt.label,
           strtok(operand_value, ","));
    break;
  case INDEXED:
    operand_value_copy = strdup(operand_value);
    ptr = strchr(operand_value_copy, '[');

    if (ptr != NULL) {
      *ptr = '\0';
    }

    strcpy(ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.label,
           operand_value_copy);

    operand_value_copy = strdup(operand_value);
    open_bracket_ptr = strchr(operand_value_copy, '[');

    if ((sscanf(open_bracket_ptr, "[%d]", &number) == 1)) {
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexOpt.number =
          number;
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexType =
          INNUMBER;
    } else {
      strcpy(
          ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexOpt.label,
          (open_bracket_ptr + 1));
      strtok(
          ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexOpt.label,
          "]");
      ast->ASTOpt.Inst.InstOperands[index].OperandOpt.Index.IndexType = INLABEL;
    }

    operand_value_copy = NULL;
    open_bracket_ptr = NULL;
    break;
  case REGISTER:
    ptr = operand_value + 1;
    ptr = strtok(ptr, ",");
    ast->ASTOpt.Inst.InstOperands[index].OperandOpt.reg = atoi(ptr);
    break;
  default:
    break;
  }
}