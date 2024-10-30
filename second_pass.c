#include "assembler.h"
#include "converter.h"
#include "errors.h"
#include <stdlib.h>

void reallocate_instructions(Translator *translator) {
  translator->code_image->instructions =
      (char **)realloc(translator->code_image->instructions,
                       (translator->code_image->count + 1) * MAX_WORD_SIZE);

  if (translator->code_image->instructions == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    exit(EXIT_FAILURE);
  }
}

void add_to_table(char symbol_name[MAX_LABEL_LENGTH], Attribute attribute,
                  int value, Symbol **table) {
  Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));

  if (new_symbol == NULL) {
    fprintf(stderr, ERROR_OUT_OF_MEMORY);
    exit(EXIT_FAILURE);
  }

  strcpy(new_symbol->symbol_name, symbol_name);
  new_symbol->attribute = attribute;
  new_symbol->value = value;
  new_symbol->next = NULL;

  if (*table == NULL) {
    *table = new_symbol;
  } else {
    while ((*table)->next != NULL) {
      table = &(*table)->next;
    }

    (*table)->next = new_symbol;
  }

  new_symbol = NULL;
}

void code_immediate_operand(Translator *translator, AST *current_node,
                            Symbol **symbol_table, int operand_index, int *line,
                            bool *has_error, const char *input_file_name) {
  if (current_node->ASTOpt.Inst.InstOperands[operand_index]
          .OperandOpt.Immediate.ImmediateType == IMLABEL) {
    Symbol *symbol_to_find =
        lookup(current_node->ASTOpt.Inst.InstOperands[1]
                   .OperandOpt.Immediate.ImmediateOpt.label,
               *symbol_table);

    if (symbol_to_find) {
      if (symbol_to_find->attribute == MDEFINE) {
        char *res = NULL;
        char *temp = NULL;
        reallocate_instructions(translator);

        res = shift_left(to_binary14(symbol_to_find->value), 2);

        temp = to_base4_encrypted(res);

        translator->code_image->instructions[translator->code_image->count] =
            temp;

        translator->code_image->count++;
        temp = NULL;
        res = NULL;
      }
    } else {
      printf(ERROR_UNDEFIND_SYMBOL,
             current_node->ASTOpt.Inst.InstOperands[operand_index]
                 .OperandOpt.Immediate.ImmediateOpt.label,
             *line, input_file_name);
      *has_error = true;
    }
  } else {
    char *res = NULL;
    char *temp = NULL;

    reallocate_instructions(translator);

    res = shift_left(
        to_binary14(current_node->ASTOpt.Inst.InstOperands[operand_index]
                        .OperandOpt.Immediate.ImmediateOpt.number),
        2);

    res[strlen(res) - 1] = '0';
    res[strlen(res) - 2] = '0';

    temp = to_base4_encrypted(res);

    translator->code_image->instructions[translator->code_image->count] = temp;

    translator->code_image->count++;

    temp = NULL;
    res = NULL;
  }
}

void code_direct_operand(Translator *translator, AST *current_node,
                         Symbol **symbol_table, int *instruction_counter,
                         int operand_index, int *line, bool *has_error,
                         const char *input_file_name) {
  Symbol *symbol_to_find = lookup(
      current_node->ASTOpt.Inst.InstOperands[operand_index].OperandOpt.label,
      *symbol_table);

  if (symbol_to_find) {
    char *res = NULL;
    char *temp = NULL;

    reallocate_instructions(translator);

    res = shift_left(to_binary14(symbol_to_find->value), 2);

    if (symbol_to_find->attribute == INTERNAL) {
      res = add_binary_numbers(res, "01");
    } else {
      res = add_binary_numbers(res, "10");
    }

    while (*symbol_table != NULL) {
      if ((*symbol_table)->attribute == EXTERNAL &&
          (strcmp((*symbol_table)->symbol_name,
                  current_node->ASTOpt.Inst.InstOperands[operand_index]
                      .OperandOpt.label) == 0)) {

        int address = *instruction_counter + 101;

        add_to_table(current_node->ASTOpt.Inst.InstOperands[operand_index]
                         .OperandOpt.label,
                     EXTERNAL, address, &translator->external_symbols);
      }

      symbol_table = &(*symbol_table)->next;
    }

    temp = to_base4_encrypted(res);
    translator->code_image->instructions[translator->code_image->count] = temp;

    translator->code_image->count++;
  } else {
    printf(
        ERROR_UNDEFIND_SYMBOL,
        current_node->ASTOpt.Inst.InstOperands[operand_index].OperandOpt.label,
        *line, input_file_name);
    *has_error = true;
  }
}

void code_register_operand(Translator *translator, AST *current_node,
                           int operand_index, int reg_offset) {
  char *reg = NULL;

  reallocate_instructions(translator);

  reg = shift_left(
      to_binary14(
          current_node->ASTOpt.Inst.InstOperands[operand_index].OperandOpt.reg),
      reg_offset);

  translator->code_image->instructions[translator->code_image->count] =
      to_base4_encrypted(reg);

  translator->code_image->count++;
}

void code_indexed_operand(Translator *translator, AST *current_node,
                          Symbol **symbol_table, int operand_index, int *line,
                          bool *has_error, const char *input_file_name) {
  char *res = NULL;

  Symbol *symbol_to_find =
      lookup(current_node->ASTOpt.Inst.InstOperands[operand_index]
                 .OperandOpt.Index.label,
             *symbol_table);

  if (symbol_to_find) {
    reallocate_instructions(translator);

    res = add_binary_numbers(shift_left(to_binary14(symbol_to_find->value), 2),
                             "10");

    translator->code_image->instructions[translator->code_image->count] =
        to_base4_encrypted(res);

    translator->code_image->count++;

    if (current_node->ASTOpt.Inst.InstOperands[operand_index]
            .OperandOpt.Index.IndexType == INLABEL) {
      Symbol *symbol_to_find =
          lookup(current_node->ASTOpt.Inst.InstOperands[operand_index]
                     .OperandOpt.Index.IndexOpt.label,
                 *symbol_table);

      if (symbol_to_find) {
        if (symbol_to_find->attribute == MDEFINE) {
          reallocate_instructions(translator);

          res = shift_left(to_binary14(symbol_to_find->value), 2);

          translator->code_image->instructions[translator->code_image->count] =
              to_base4_encrypted(res);

          translator->code_image->count++;
        }
      } else {
        printf(ERROR_UNDEFIND_SYMBOL,
               current_node->ASTOpt.Inst.InstOperands[operand_index]
                   .OperandOpt.Index.IndexOpt.label,
               *line, input_file_name);
        *has_error = true;
      }
    } else if (current_node->ASTOpt.Inst.InstOperands[operand_index]
                   .OperandOpt.Index.IndexType == INNUMBER) {
      reallocate_instructions(translator);

      res = shift_left(
          to_binary14(current_node->ASTOpt.Inst.InstOperands[operand_index]
                          .OperandOpt.Index.IndexOpt.number),
          2);

      translator->code_image->instructions[translator->code_image->count] =
          to_base4_encrypted(res);

      translator->code_image->count++;
    }
  } else {
    printf(ERROR_UNDEFIND_SYMBOL,
           current_node->ASTOpt.Inst.InstOperands[operand_index]
               .OperandOpt.Index.label,
           *line, input_file_name);
    *has_error = true;
  }
}

void code_inst_operand(Translator *translator, AST *current_node,
                       Symbol **symbol_table, int *instruction_counter,
                       int operand_index, int reg_offset, int *line,
                       bool *has_error, const char *input_file_name) {
  if (current_node->ASTOpt.Inst.InstOperands[operand_index].OperandType ==
      IMMEDIATE) {
    code_immediate_operand(translator, current_node, symbol_table,
                           operand_index, line, has_error, input_file_name);
  } else if (current_node->ASTOpt.Inst.InstOperands[operand_index]
                 .OperandType == DIRECT) {
    code_direct_operand(translator, current_node, symbol_table,
                        instruction_counter, operand_index, line, has_error,
                        input_file_name);
  } else if (current_node->ASTOpt.Inst.InstOperands[operand_index]
                 .OperandType == REGISTER) {
    code_register_operand(translator, current_node, operand_index, reg_offset);
  } else if (current_node->ASTOpt.Inst.InstOperands[operand_index]
                 .OperandType == INDEXED) {
    code_indexed_operand(translator, current_node, symbol_table, operand_index,
                         line, has_error, input_file_name);
  }
}

void handle_directive(Translator *translator, AST *current_node,
                      Symbol **symbol_table, int *data_counter, int *line,
                      bool *has_error, const char *input_file_name) {
  int i = 0;
  if (current_node->ASTOpt.Dir.DirOpt == ENTRY) {
    Symbol *symbol_to_find =
        lookup(current_node->ASTOpt.Dir.ParamsOpt.label, *symbol_table);
    if (symbol_to_find) {
      symbol_to_find->attribute = INTERNAL;
    }
  } else if (current_node->ASTOpt.Dir.DirOpt == DATA) {
    for (i = 0; i < current_node->ASTOpt.Dir.ParamsOpt.Data.count; i++) {
      reallocate_instructions(translator);
      if (!is_integer(current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i])) {
        Symbol *symbol_to_find = lookup(
            current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i], *symbol_table);
        if (symbol_to_find) {
          translator->code_image->instructions[translator->code_image->count] =
              to_base4_encrypted(to_binary14(symbol_to_find->value));
          translator->code_image->count++;
          (*data_counter)++;
        } else {
          printf(ERROR_UNDEFIND_SYMBOL,
                 current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i], *line,
                 input_file_name);
          *has_error = true;
        }
      } else {
        translator->code_image->instructions[translator->code_image->count] =
            to_base4_encrypted(to_binary14(
                atoi(current_node->ASTOpt.Dir.ParamsOpt.Data.elements[i])));
        translator->code_image->count++;
        (*data_counter)++;
      }
    }
  } else if (current_node->ASTOpt.Dir.DirOpt == STRING) {
    for (i = 0; i <= strlen(current_node->ASTOpt.Dir.ParamsOpt.string); i++) {
      reallocate_instructions(translator);
      translator->code_image->instructions[translator->code_image->count] =
          to_base4_encrypted(
              to_binary14(current_node->ASTOpt.Dir.ParamsOpt.string[i]));
      translator->code_image->count++;
      (*data_counter)++;
    }
  }
}

void handle_instruction(Translator *translator, AST *current_node,
                        Symbol **symbol_table, int *instruction_counter,
                        int reg_offset, int *line, bool *has_error,
                        const char *input_file_name) {

  char *binary_inst_code = NULL;
  char *res = NULL;
  int i = 0;

  reallocate_instructions(translator);

  binary_inst_code =
      shift_left(to_binary14(current_node->ASTOpt.Inst.InstType), 6);

  switch (current_node->ASTOpt.Inst.InstType) {
  case RTS:
  case HLT: {
    translator->code_image->instructions[translator->code_image->count] =
        to_base4_encrypted(binary_inst_code);

    translator->code_image->count++;
    break;
  }

  case NOT:
  case CLR:
  case INC:
  case DEC:
  case JMP:
  case BNE:
  case RED:
  case PRN:
  case JSR: {
    char *binary_operand_code = shift_left(
        to_binary14(current_node->ASTOpt.Inst.InstOperands[1].OperandType), 2);

    res = to_base4_encrypted(
        add_binary_numbers(binary_inst_code, binary_operand_code));

    translator->code_image->instructions[translator->code_image->count] = res;
    translator->code_image->count++;

    break;
  }

  case MOV:
  case CMP:
  case ADD:
  case SUB:
  case LEA: {
    char *operand1 = shift_left(
        to_binary14(current_node->ASTOpt.Inst.InstOperands[0].OperandType), 4);

    char *operand2 = shift_left(
        to_binary14(current_node->ASTOpt.Inst.InstOperands[1].OperandType), 2);

    char *sum_operands = add_binary_numbers(operand1, operand2);
    res = add_binary_numbers(binary_inst_code, sum_operands);
    res = to_base4_encrypted(res);

    translator->code_image->instructions[translator->code_image->count] = res;

    translator->code_image->count++;
    break;
  }
  }

  switch (current_node->ASTOpt.Inst.InstType) {
  case RTS:
  case HLT:
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
    code_inst_operand(translator, current_node, symbol_table,
                      instruction_counter, 1, reg_offset, line, has_error,
                      input_file_name);
    break;
  }
  case MOV:
  case CMP:
  case ADD:
  case SUB:
  case LEA: {
    if (current_node->ASTOpt.Inst.InstOperands[0].OperandType == REGISTER &&
        current_node->ASTOpt.Inst.InstOperands[1].OperandType == REGISTER) {
      void *source_reg = shift_left(
          to_binary14(current_node->ASTOpt.Inst.InstOperands[0].OperandOpt.reg),
          5);
      void *dest_reg = shift_left(
          to_binary14(current_node->ASTOpt.Inst.InstOperands[1].OperandOpt.reg),
          2);

      res = add_binary_numbers(source_reg, dest_reg);
      translator->code_image->instructions[translator->code_image->count] =
          to_base4_encrypted(res);
      translator->code_image->count++;
    } else {
      reg_offset = 5;

      for (i = 0; i < 2; i++) {
        code_inst_operand(translator, current_node, symbol_table,
                          instruction_counter, i, reg_offset, line, has_error,
                          input_file_name);

        reg_offset = 2;
      }
    }

    break;
  }
  }

  *instruction_counter = translator->code_image->count;
}

bool do_second_pass(Translator **translator, Symbol **symbol_table,
                    const char *am_file_name, FILE *am_file,
                    int *instruction_counter, int *data_counter,
                    const char *input_file_name) {
  char line[MAX_LINE_LENGTH] = {0};
  bool has_error = false;
  int current_line = 0;
  int reg_offset = 0;

  Tokens *tokens = NULL;
  AST *current_node = NULL;

  *translator = (Translator *)malloc(sizeof(Translator));
  (*translator)->code_image = (CodeImage *)malloc(sizeof(CodeImage));
  (*translator)->code_image->instructions = NULL;
  (*translator)->code_image->count = 0;

  (*translator)->external_symbols = NULL;
  (*translator)->internal_symbols = NULL;

  while (fgets(line, sizeof(line), am_file)) {
    tokens = split_line_to_tokens(line);

    current_line++;
    current_node = parse_tokens(tokens, current_line, input_file_name);

    if (current_node->ASTType == EMPTY || current_node->ASTType == COMMENT) {
      continue;
    }

    if (current_node->ASTType == DIRECTIVE) {
      handle_directive(*translator, current_node, symbol_table, data_counter,
                       &current_line, &has_error, input_file_name);
    } else if (current_node->ASTType == INSTRUCTION) {
      handle_instruction(*translator, current_node, symbol_table,
                         instruction_counter, reg_offset, &current_line,
                         &has_error, input_file_name);
    }

    free_ast(current_node);
    free_tokens(tokens);
    current_node = NULL;
    tokens = NULL;
  }

  while (*symbol_table != NULL) {
    if ((*symbol_table)->attribute == INTERNAL) {
      reallocate_instructions(*translator);

      add_to_table((*symbol_table)->symbol_name, INTERNAL,
                   (*symbol_table)->value, &(*translator)->internal_symbols);
    }

    symbol_table = &(*symbol_table)->next;
  }

  return has_error;
}