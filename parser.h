#define MAX_LABEL_LENGTH 20

typedef struct AST {
  char *syntax_error;
  char label_name[MAX_LABEL_LENGTH + 1];

  enum { INST, DIR, COMM, DEFINE, EMPTY, ERROR } ASTType;

  union {
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

      struct {
        enum {
          INST_NUMBER,      // immediate addressing
          INST_LABEL,       // direct addressing
          INST_INDEX_CONST, // indexed addressing (const)
          INST_INDEX_LABEL, // indexed addressing (symbolic)
          INST_REGISTER     // register addressing
        } OperandType;

        union {
          int number;
          char *label;
          int reg;
          struct {
            char *label;

            union {
              int number;
              char *label;
            } IndexOpt;
          } Index;
        } OperandOpt;
      } InstOperands[2];
    } Inst;

    struct {
      enum { DATA, STRING, ENTRY, EXTERN } DirOpt;

      union {
        char *label_name;
        char *string;
        struct {
          enum { DATA_LABEL, DATA_NUMBER } DataType;

          union {
            char *label;
            int number;
          } DataOpt;
        } *Data;
      } Dir_Operand;
    } Dir;

    struct {
      char *comment;
    } Comm;

    struct {
      const char *name;
      int number;
    } Define;
  } ASTOpt;
} AST;

AST *get_ast_from_line(const char *line);