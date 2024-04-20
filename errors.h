/* Operand type error: the ___ addresing mode of the source operand ___ is not supported for the ___ instruction
 * instruction */

/* Operand type error: the ___addresing mode of the destination operand ___ is not supported for the ___ instruction
 * instruction */

#ifndef ERRORS_H
#define ERRORS_H

/* Syntax errors */
#define ERROR_UNEXPECTED_COMMA_AFTER_KEYWORD "ERROR: Unexpected comma after keyword '%s' on line %d in file %s\n"
#define ERROR_LABEL_NAME_TOO_LONG "ERROR: Label '%s' is longer than '%d' characters on line %d in file %s\n"
#define ERROR_LABEL_CANNOT_START_WITH_NUM "ERROR: Label name can't start with number on line %d in file %s\n"
#define ERROR_LABEL_NAME_NOT_LETTER_OR_NUM "ERROR: Label names must be alphanumeric, violation on line %d in file %s\n"
#define ERROR_LABEL_NAME_IS_KEYWORD "ERROR: Label name can't be equal to a keyword '%s' on line %d in file %s\n"
#define ERROR_WHITESPACE_AFTER_LABEL "ERROR: Whitespace between label name and colon on line %d in file %s, label ignored\n"
/*#define ERROR_INVALID_DIRECTIVE_DEFINITION "ERROR: Invalid directive definition: %s on line %d in file %s\n"*/
#define ERROR_EXPECTED_LABEL "ERROR: Expected a label after the directive '%s' on line %d in file %s\n"
#define ERROR_INVALID_DATA_DIRECTIVE "ERROR: Invalid data directive on line %d in file %s\n"
#define ERROR_UNEXPECTED_COMMA "ERROR: Unexpected comma on line %d in file %s\n"
#define ERROR_EXTRA_COMMA_AFTER_NUMBER "ERROR: Extra comma after a number '%s' on line %d in file %s\n"
#define ERROR_MISSING_COMMA_BETWEEN_NUMBERS "ERROR: Missing comma between numbers '%s' and '%s' on line %d in file%s\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_DIRECTIVE "ERROR: Unexpected comma at the end of the directive definition on line %d in file %s\n"
#define ERROR_INVALID_DEFINE_DEFINITION "ERROR: Invalid define definition: '%S' on line %d in file %s\n"
#define ERROR_EXPECTED_NUMBER_AFTER_EQUAL_SIGN "ERROR: Expected a number after the '='"
#define ERROR_EXPECTED_EQUAL_SIGN_AFTER_DEFINE_NAME "ERROR: Expected an '=' after the define name"
#define ERROR_EXPECTED_NAME_AFTER_DEFINE "ERROR: Expected a name after .define keyword"
#define ERROR_INVALID_STRING_DEFINITION "ERROR: Invalid string definition: '%s' on line %d in file %s\n"
#define ERROR_EXPECTED_STRING_QUOTES "ERROR: Expected a string enclosed in quotes"
#define ERROR_EXPECTED_NAME_AFTER_STRING "ERROR: Expected a name after the .string keyword"
#define ERROR_INVALID_INSTRUCTION "ERROR: Invalid instruction '%s' on line %d in file %s\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_INSTRUCTION "ERROR: Unexpected comma at the end of the %s definition on line %d in file %s\n"
#define ERROR_EXPECTED_COMMA_AFTER_OPERAND "ERROR: The instruction '%s' expects a comma after the %s operand on line %d in file %s\n"
#define ERROR_UNEXPECTED_COMMA_AFTER_OPERAND "ERROR: The instruction '%s' has unnecessary comma after the '%s' operand on line %d in file %s\n"
#define ERROR_UNEXPECTED_COMMAS_AFTER_OPERAND "ERROR: The instruction '%s' has to many commas after the '%s' operand on line %d in file %s\n"
#define ERROR_UNEXPECTED_OPERANDS_AFTER_INSTRUCTION "ERROR: The instruction '%s' has too many operands: expected '%d' on line %d in file %s\n"
#define ERROR_EXPECTED_OPERANDS_AFTER_INSTRUCTION "ERROR: The instruction '%s' expects '%d' operands on line %d in file %s\n"
#define ERROR_INVALID_OPERAND "ERROR: The instruction '%s' does not support the operand '%s' on line %d in file %s\n"
#define ERROR_INVALID_OPERAND_TYPE "ERROR: The type of the '%s' operand '%s' is invalid for the instruction '%s' on line %d in file %s\n"
#define ERROR_UNEXPECTED_INSTRUCTION_OPERANDS "ERROR: The instruction '%s' does not support the operands on line %d in file %s\n"


/* Files */
#define ERROR_NO_INPUT_FILES "ERROR: Missing input files\n"
#define ERROR_CANNOT_READ "ERROR: Cannot read the file: %s\n"
#define ERROR_CANNOT_WRITE "ERROR: Cannot write the file: %s\n"
#define ERROR_LINE_TOO_LONG "WARN: Line number %d longer than the max allowed (%d)\n"

/* Labels */
#define ERROR_EMPTY_LABEL "WARN: Empty label on line %d in file %s\n"
#define ERROR_USELESS_LABEL "WARN: Useless label '%s', no directive or operation on line %d in file %s\n"
#define ERROR_LABEL_NOT_ON_START_OF_LINE "ERROR: Labels not at the start of the line on line %d in file %s, label ignored\n"
#define ERROR_LABEL_DOESNT_EXIST "ERROR: Label \"%s\" does not exist on line %d in file %s\n"


/* Assembly directives */
#define ERROR_EMPTY_DIRECTIVE "ERROR: Empty directive on line %d in file %s\n"
#define ERROR_UNKNOWN_DIRECTIVE "ERROR: Unknown directive '%s' on line %d in file %s\n"
#define ERROR_DATA_OUT_OF_BOUNDS "ERROR: Value %ld is out of bounds (%d..%d) on line %d in file %s\n"
#define ERROR_INVALID_CHARACTERS "ERROR: Invalid characters \"%s\" on line %d in file %s\n"

/* OPs */
#define ERROR_UNKNOWN_OP "ERROR: Unknown OP name '%s' on line %d in file %s\n"
#define ERROR_MISSING_FIRST_OPERAND "ERROR: Invalid first operand on line %d in file %s\n"
#define ERROR_MISSING_SECOND_OPERAND "ERROR: Invalid second operand on line %d in file %s\n"
#define ERROR_CODE_LABEL_IN_MATRIX "ERROR: Label \"%s\" pointing to code found in matrix on line %d in file %s\n"
#define ERROR_UNSUPPORTED_OPERAND_TYPE "ERROR: This instruction (%s) does not support this type of operator (%s) on line %d in file %s\n"

/* Parsing */
#define ERROR_EXPECTED_CHARACTER "ERROR: Expected '%c' but got '%c' (ASCII %d) on line %d in file %s\n"
#define ERROR_EXPECTED_CHARACTER_EOL "ERROR: Expected '%c' but line ended on line %d in file %s\n"
#define ERROR_EXPECTED_ANY_CHARACTER_EOL "ERROR: Expected a character but line ended on line %d in file %s\n"
#define ERROR_EXPECTED_NUMBER "ERROR: Expected a number but got '%c' (ASCII %d) on line %d in file %s\n"
#define ERROR_EXPECTED_NUMBER_EOL "ERROR: Expected a number but line ended on line %d in file %s\n"
#define ERROR_EXPECTED_WHITESPACE "ERROR: Expected whitespace but got '%c' (ASCII %d) on line %d in file %s\n"
#define ERROR_EXPECTED_EOL "ERROR: Expected end of line, got \"%s\" on line %d in file %s\n"
#define ERROR_MATRIX_DIMENSION_POSITIVE "ERROR: Matrix dimension %ld must be positive on line %d in file %s\n"
#define ERROR_MATRIX_DIMENSION_OUT_OF_BOUNDS "ERROR: Matrix dimension %ld is out of bounds (1..1024) on line %d in file %s\n"
#define ERROR_REGISTER_OUT_OF_BOUNDS "ERROR: Register number %ld is out of bounds (%d..%d) on line %d in file %s\n"
#define ERROR_LABEL_EXISTS "ERROR: Label \"%s\" already exists on line %d in file %s\n"


/* General errors */
#define ERROR_OUT_OF_MEMORY "FATAL: MEMORY ALLOCATION FAILED\n"

#endif
