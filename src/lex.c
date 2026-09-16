#include "kit.h"

bool lex_spaces(file_t* file) {
    file->tok = (tok_t){0};
    char* ptr = file->ptr-1;
    while (*++ptr) {
        if (isspace(*ptr)) continue;
        if (*ptr == '#') {
            // char* start = ptr;
            while (*++ptr&&*ptr!='\n');
            if (!*ptr||!*(ptr+1)) return file->tok.type++;
            continue;
        }
        break;
    }
    if (!*ptr) return file->tok.type++;
    file->ptr = ptr;
    return 1;
}

bool lex_identifier(file_t* file) {
    if (!lex_spaces(file)) return 0;
    char* ptr = file->ptr;
    if (isalpha(*ptr)||*ptr=='_') {
        while (*++ptr) if (isalnum(*ptr)||*ptr=='_') continue; else break;
        if (!*ptr) return file->tok.type++;

        file->tok.len  = (size_t)(ptr-file->ptr);
        file->tok.ptr  = file->ptr;
        file->tok.type = identifier_tok;
        return 1;
    }
    return file->tok.type++;
}

bool lex_string(file_t* file) {
    if (!lex_spaces(file)) return 0;
    char* ptr = file->ptr;
    if (*ptr=='=') {

    }
    return 0;
}

bool lex_func(file_t* file) {


    return 1;
}

bool lex_postfix(file_t* file) {
    tok_t parent = file->tok;
    char* ptr = file->ptr;
    while (*ptr) {
        if (!lex_spaces(file)) return 0;
        
                
    }
    return 1;
}

bool lex_value(file_t* file) {
    char* ptr = file->ptr;
    if ((ptr[0]=='+'||ptr[0]=='-')&&ptr[0]==ptr[1]) { // ++arr, --arr
        if (!lex_identifier(file)) return 0;
        tok_t tok = file->tok; tok.unary.present++;
        if (ptr[0]=='-') tok.unary.plus_minus++;
        // ++arr[0], ++arr()
        char* ptr = file->ptr;
        if (!lex_postfix(file)) return 0;
    }

    return 1;
}

bool lex_expresion(file_t* file) {
    if (!lex_value(file)) return 0;

    return 1;
}