#include "kit.h"

tok_t* lex_operator(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    tok.file = top_file(proj);
    tok.ptr = tok.file->ptr;
    tok.len = 1;
    // if (isalnum(tok.ptr[0])) return NULL;

    switch (tok.ptr[0]) {
        case '{': tok.type=curly_open; break;
        case '}': tok.type=curly_close; break;
        case '(': tok.type=parentheses_open; break;
        case ')': tok.type=parentheses_close; break;
        case ';': tok.type=semicolon; break;

        default:
            if (proj->optional_tok) return NULL;
            tokdup(proj, &tok);
            return error(proj, "unexpected operator");
    }

    tok.file->ptr++;

    return tokdup(proj, &tok);
}