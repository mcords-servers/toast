#include "kit.h"

tok_t* lex_identifier(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    tok.file = top_file(proj);
    tok.ptr = tok.file->ptr;
    if (!isalpha(tok.file->ptr[0])&&tok.file->ptr[0]!='_') {
        if (proj->optional_tok) return NULL;
        proj->last_tok=tok;
        return error(proj, "identifier was expected");
    }
    while (isalnum(tok.file->ptr[0])||tok.file->ptr[0]=='_') tok.file->ptr++;
    tok.len = tok.file->ptr-tok.ptr;
    tok.type = identifier_tok;

    if (tok.len<=8) {
        size_t val = 0;
        memcpy(&val, tok.ptr, tok.len);
        if (val==s8("func")) tok.type = func_tok;
        if (val==s8("if")) tok.type = if_tok;
        if (val==s8("else")) tok.type = else_tok;
        if (val==s8("elif")) tok.type = elif_tok;
        if (val==s8("return")) tok.type = return_tok;
        if (val==s8("break")) tok.type = break_tok;
        if (val==s8("task")) tok.type = task_tok;
        if (val==s8("while")) tok.type = while_tok;
        if (val==s8("for")) tok.type = for_tok;
        if (val==s8("class")) tok.type = class_tok;
        if (val==s8("enum")) tok.type = enum_tok;
        if (val==s8("float")) tok.type = float_tok;
        if (val==s8("double")) tok.type = double_tok;
        if (val==s8("int")) tok.type = int_tok;
        if (val==s8("uint")) tok.type = uint_tok;
        if (val==s8("long")) tok.type = long_tok;
        if (val==s8("ulong")) tok.type = ulong_tok;

        if (tok.len<=4&&((val&0xFF)==s8("u")||(val&0xFF)==s8("i"))) {
            size_t num = val>>8;
            if (num==s8("8")) tok.type = type_tok;
            if (num==s8("16")) tok.type = type_tok;
            if (num==s8("32")) tok.type = type_tok;
            if (num==s8("64")) tok.type = type_tok;
            if (num==s8("128")) tok.type = type_tok;
            if (num==s8("256")) tok.type = type_tok;
        }
    }

    return tokdup(proj, &tok);
}
