#include "assembler.h"
#include "errors.h"
#include "symbol_table.h"

Symbol *lookup(char symbol_name[MAX_LABEL_LENGTH], Symbol *current) {
  while (current != NULL) {
    if (strcmp(current->symbol_name, symbol_name) == 0) {
      return current;
    }

    current = current->next;
  }

  return NULL;
}

void add_symbol(char symbol_name[MAX_LABEL_LENGTH], Attribute attribute,
                int value, Symbol *new_symbol, Symbol **table) {
  strcpy(new_symbol->symbol_name, symbol_name);
  new_symbol->attribute = attribute;
  new_symbol->value = value;

  if (*table == NULL) {
    *table = new_symbol;
  } else {
    Symbol *current_symbol = *table;
    while (current_symbol->next != NULL) {
      current_symbol = current_symbol->next;
    }

    current_symbol->next = new_symbol;
  }
}

int get_tokens_count(AST **current_node) {
  int token_counter = 0;
  int i = 0;

  switch ((*current_node)->ASTOpt.Inst.InstType) {
  case RTS:
  case HLT:
    token_counter += 1;
    break;

  case NOT:
  case CLR:
  case INC:
  case DEC:
  case JMP:
  case BNE:
  case RED:
  case PRN:
  case JSR: {
    if ((*current_node)->ASTOpt.Inst.InstOperands[1].OperandType == INDEXED) {
      token_counter += 3;
    } else {
      token_counter += 2;
    }

    break;
  }

  case MOV:
  case CMP:
  case ADD:
  case SUB:
  case LEA: {
    if ((*current_node)->ASTOpt.Inst.InstOperands[0].OperandType == REGISTER &&
        (*current_node)->ASTOpt.Inst.InstOperands[1].OperandType == REGISTER) {
      token_counter += 1;
    } else {
      for (i = 0; i < 2; i++) {
        if ((*current_node)->ASTOpt.Inst.InstOperands[i].OperandType ==
            INDEXED) {
          token_counter += 2;
        } else {
          token_counter += 1;
        }
      }
    }

    token_counter += 1;
    break;
  }
  }

  return token_counter;
}

bool do_first_pass(Symbol **symbol_table, FILE *am_file,
                   const char *input_file_name) {
  char line[MAX_LINE_LENGTH] = {0};
  int max_lines = MAX_MEMORY_SIZE / MAX_WORD_SIZE;
  int instruction_counter = 100;
  int data_counter = 0;
  bool has_error = false;
  int current_line = 0;
  int i = 0;

  Tokens *tokens = NULL;
  AST *current_node = NULL;
  Symbol *new_symbol = NULL;

  am_file = fopen(input_file_name, "r");

  if (!am_file) {
    fprintf(stderr, ERROR_CANNOT_READ, input_file_name);
    exit(EXIT_FAILURE);
  }

  while (fgets(line, sizeof(line), am_file)) {
    tokens = split_line_to_tokens(line);

    current_line++;
    current_node = parse_tokens(tokens, current_line, input_file_name);

    if (current_line >= max_lines) {
      printf(ERROR_MEMORY_OVERFLOW, input_file_name, max_lines);
      has_error = true;
      break;
    }

    if (current_node->ASTType == EMPTY || current_node->ASTType == COMMENT) {
      continue;
    }

    if (current_node->ASTType == ERROR) {
      printf("%s", current_node->syntax_error);
      has_error = true;
      continue;
    }

    new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->next = NULL;

    if (current_node->ASTType == DEFINE) {
      if (lookup(current_node->ASTOpt.Define.name, *symbol_table)) {
        printf(ERROR_REDEFINITION_OF_SYMBOL, current_node->ASTOpt.Define.name,
               current_line, input_file_name);
        has_error = true;
      } else {
        add_symbol(current_node->ASTOpt.Define.name, MDEFINE,
                   current_node->ASTOpt.Define.number, new_symbol,
                   symbol_table);
      }
    } else if (current_node->ASTType == DIRECTIVE) {
      if (current_node->ASTOpt.Dir.DirOpt == DATA ||
          current_node->ASTOpt.Dir.DirOpt == STRING) {
        if (lookup(current_node->label_name, *symbol_table)) {
          printf(ERROR_REDEFINITION_OF_SYMBOL, current_node->label_name,
                 current_line, input_file_name);
          has_error = true;
        } else {
          if (strcmp(current_node->label_name, "") != 0) {
            if (current_node->ASTOpt.Dir.DirOpt == DATA) {
              data_counter += current_node->ASTOpt.Dir.ParamsOpt.Data.count;

              for (i = 0; i < current_node->ASTOpt.Dir.ParamsOpt.Data.count;
                   i++) {
                if (!is_integer(
                        current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i])) {
                  Symbol *symbol_to_find = lookup(
                      current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i],
                      *symbol_table);

                  if (symbol_to_find != NULL) {
                    if (symbol_to_find->attribute != MDEFINE) {
                      printf(
                          ERROR_INVALID_DATA_ELEMENT_TYPE,
                          current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i],
                          current_line, input_file_name);
                      has_error = true;
                    }
                  } else {
                    printf(ERROR_UNDEFIND_DATA_SYMBOL_ELEMENT,
                           current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i],
                           current_line, input_file_name);
                    has_error = true;
                  }
                }
              }
            } else {
              for (i = 0;
                   i <= strlen(current_node->ASTOpt.Dir.ParamsOpt.string);
                   i++) {
                data_counter++;
              }
            }

            add_symbol(current_node->label_name, MDATA, instruction_counter,
                       new_symbol, symbol_table);

            instruction_counter += data_counter;
            data_counter = 0;
          }
        }
      } else if (current_node->ASTOpt.Dir.DirOpt == EXTERN) {
        if (lookup(current_node->ASTOpt.Dir.ParamsOpt.label, *symbol_table)) {
          printf(ERROR_REDEFINITION_OF_SYMBOL,
                 current_node->ASTOpt.Dir.ParamsOpt.label, current_line,
                 input_file_name);
          has_error = true;
        } else {
          add_symbol(current_node->ASTOpt.Dir.ParamsOpt.label, EXTERNAL, 0,
                     new_symbol, symbol_table);
        }
      }
    } else if (current_node->ASTType == INSTRUCTION) {
      if ((strcmp(current_node->label_name, "") != 0)) {
        if (lookup(current_node->label_name, *symbol_table)) {
          printf(ERROR_REDEFINITION_OF_SYMBOL, current_node->label_name,
                 current_line, input_file_name);
          has_error = true;
        } else {
          add_symbol(current_node->label_name, CODE, instruction_counter,
                     new_symbol, symbol_table);
        }

        instruction_counter += get_tokens_count(&current_node);
      } else {
        instruction_counter += get_tokens_count(&current_node);
      }
    }

    free_ast(current_node);
    free_tokens(tokens);
    current_node = NULL;
    tokens = NULL;
  }

  return has_error;
}