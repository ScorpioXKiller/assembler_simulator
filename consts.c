#include "consts.h"

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