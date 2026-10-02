#ifndef STRUCTS_H
#define STRUCTS_H

#include "kit.h"

typedef struct token tok_t;
typedef struct file {
    char* path;
    size_t len;
    char* buffer;
    char* ptr;

    tok_t** globals;
} file_t;

typedef struct token {
    size_t len;
    char* ptr;

    file_t* file;
    enum : uint64_t {
        func_holder, class_holder,
        identifier_tok, func_tok, if_tok,
        else_tok, elif_tok, return_tok, break_tok,
        task_tok, while_tok, for_tok, class_tok, enum_tok,
        type_tok, float_tok, double_tok, int_tok, long_tok, ulong_tok, uint_tok,
            curly_open, curly_close, parentheses_open, parentheses_close, semicolon,
        declaration_holer,
    } type;
    tok_t** tokens;
} tok_t;

typedef struct project {
    char*    path;
    file_t** files;
    file_t** files_stack;
    tok_t**  stack;
    tok_t**  globals;

    tok_t*   main_func;
    tok_t    last_tok;

    bool     optional_tok;
    jmp_buf  jump_buffer;
} proj_t;

#endif