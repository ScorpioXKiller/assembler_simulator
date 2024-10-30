/**
 * @file errors.h
 * @brief This file contains error and warning message definitions used throughout the program.
 */

#ifndef ERRORS_H
#define ERRORS_H

/* Syntax errors */
#define ERROR_MACRO_NAME "ERROR: Macro name '%s' cannot be a keyword on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_KEYWORD "ERROR: Unexpected comma after keyword '%s' on line '%d' in file '%s'\n\n"
#define ERROR_LABEL_NAME_TOO_LONG "ERROR: Label '%s' is longer than '%d' characters on line '%d' in file '%s'\n\n"
#define ERROR_LABEL_CANNOT_START_WITH_NUM "ERROR: Label name can't start with number on line '%d' in file '%s'\n\n"
#define ERROR_LABEL_NAME_NOT_LETTER_OR_NUM "ERROR: Label names must be alphanumeric, violation on line '%d' in file '%s'\n\n"
#define ERROR_LABEL_NAME_IS_KEYWORD "ERROR: Label name can't be equal to a keyword '%s' on line '%d' in file '%s'\n\n"
#define ERROR_WHITESPACE_AFTER_LABEL "ERROR: Whitespace between label name and colon on line '%d' in file '%s', label ignored\n\n"
#define WARN_LABEL_IGNORED "WARN: Label '%s' ignored on line '%d' in file '%s'\n\n"
#define ERROR_EXPECTED_LABEL "ERROR: Expected a label after the directive '%s' on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_COMMA "ERROR: Unexpected comma on line '%d' in file '%s'\n\n"
#define ERROR_EXTRA_COMMA_AFTER_NUMBER "ERROR: Extra comma after a number '%s' on line %d in file '%s'\n\n"
#define ERROR_MISSING_COMMA_BETWEEN_NUMBERS "ERROR: Missing comma between numbers '%s' and '%s' on line '%d' in file '%s\n\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_DIRECTIVE "ERROR: Unexpected comma at the end of the directive definition on line '%d' in file '%s'\n\n"
#define ERROR_INVALID_DEFINE_DEFINITION "ERROR: Invalid define definition: '%S' on line '%d' in file '%s'\n\n"
#define ERROR_EXPECTED_NUMBER_AFTER_EQUAL_SIGN "ERROR: Expected a number after the '='"
#define ERROR_EXPECTED_EQUAL_SIGN_AFTER_DEFINE_NAME "ERROR: Expected an '=' after the define name"
#define ERROR_EXPECTED_NAME_AFTER_DEFINE "ERROR: Expected a name after .define keyword"
#define ERROR_INVALID_STRING_DEFINITION "ERROR: Invalid string definition: '%s' on line '%d' in file '%s'\n\n"
#define ERROR_EXPECTED_STRING_QUOTES "ERROR: Expected a string enclosed in quotes"
#define ERROR_EXPECTED_NAME_AFTER_STRING "ERROR: Expected a name after the .string keyword"
#define ERROR_INVALID_INSTRUCTION "ERROR: Invalid instruction '%s' on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_INSTRUCTION "ERROR: Unexpected comma at the end of the '%s' definition on line '%d' in file '%s'\n\n"
#define ERROR_EXPECTED_COMMA_AFTER_OPERAND "ERROR: The instruction '%s' expects a comma after the '%s' operand on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_OPERAND "ERROR: The instruction '%s' has unnecessary comma after the '%s' operand on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_COMMAS_AFTER_OPERAND "ERROR: The instruction '%s' has to many commas after the '%s' operand on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_OPERANDS_AFTER_INSTRUCTION "ERROR: The instruction '%s' has too many operands: expected '%d' on line '%d' in file '%s'\n\n"
#define ERROR_EXPECTED_OPERANDS_AFTER_INSTRUCTION "ERROR: The instruction '%s' expects '%d' operands on line '%d' in file '%s'\n\n"
#define ERROR_INVALID_OPERAND "ERROR: The instruction '%s' does not support the operand '%s' on line '%d' in file '%s'\n\n"
#define ERROR_INVALID_OPERAND_TYPE "ERROR: The type of the '%s' operand '%s' is invalid for the instruction '%s' on line '%d' in file '%s'\n\n"
#define ERROR_UNEXPECTED_INSTRUCTION_OPERANDS "ERROR: The instruction '%s' does not support the operands on line '%d' in file '%s'\n\n"
#define ERROR_INVALID_DATA_ELEMENT "ERROR: Invalid data element '%s' on line '%d' in file '%s'\n\n"
#define ERROR_INVALID_DATA_ELEMENT_TYPE "ERROR: Invalid data element type '%s' on line '%d' in file '%s'\n\n"
#define ERROR_UNDEFIND_DATA_SYMBOL_ELEMENT "ERROR: Undefind data symbol element '%s' on line '%d' in file '%s'\n\n"

/* Files */
#define ERROR_MISSING_FILE_NAME "ERROR: Missing the file name\n\n"
#define ERROR_CANNOT_READ "ERROR: Cannot read the file: '%s'\n\n"
#define ERROR_CANNOT_WRITE "ERROR: Cannot write the file: '%s'\n\n"
#define ERROR_LINE_TOO_LONG "WARN: Line number '%d' longer than the max allowed '%d'\n\n"

/* General errors */
#define ERROR_UNDEFIND_SYMBOL "ERROR: Symbol '%s' declared but was never defined: on line '%d' in file '%s'\n\n"
#define ERROR_REDEFINITION_OF_SYMBOL "ERROR: Redefinition of symbol '%s' on line '%d' in file '%s'\n\n"
#define ERROR_OUT_OF_MEMORY "FATAL: MEMORY ALLOCATION FAILED\n\n"
#define ERROR_MEMORY_OVERFLOW "ERROR: Memory overflow: too many lines in file '%s', max is '%d'\n\n"

#endif