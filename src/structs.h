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
        identifier_tok, func_tok, if_tok,
        else_tok, elif_tok, return_tok, break_tok,
        task_tok, while_tok, for_tok, class_tok, enum_tok,
    } type;
    tok_t** tokens;
} tok_t;

typedef struct project {
    file_t** files;
    file_t** files_stack;
    tok_t**  stack;
    tok_t**  globals;

    tok_t*   main_func;
} proj_t;

#endif