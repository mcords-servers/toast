#include <kit.h>

bool lex_spaces(proj_t* proj) {
    file_t* file = top_file(proj);
loop:
    while (isspace(file->ptr[0])) file->ptr++;
    if (!file->ptr[0]) return false;
    if (file->ptr[0]=='#') {
        // TODO: Implement 'require' here and other fAnCy StUFf
        while (file->ptr[0]&&file->ptr[0]!='\n') file->ptr++;
        goto loop;
    }
    return true;
}

tok_t* lex_operator(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    tok.file = top_file(proj);
    tok.ptr = tok.file->ptr;
    tok.len = 1;

    switch (tok.ptr[0]) {
        case '{': tok.type=curly_open;
        case '}': tok.type=curly_close;
        case '(': tok.type=parentheses_open;
        case ')': tok.type=parentheses_close;
        break;

        default:
            (proj->last_tok=allocpy(&tok, sizeof(tok_t)));
            return error(proj, "unexpected operator");
    }

    tok.file->ptr++;

    return (proj->last_tok=allocpy(&tok, sizeof(tok_t)));
}

tok_t* lex_identifier(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    tok.file = top_file(proj);
    tok.ptr = tok.file->ptr;
    if (!isalpha(tok.file->ptr[0])&&tok.file->ptr[0]!='_') return NULL;
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

    return (proj->last_tok=allocpy(&tok, sizeof(tok_t)));
}

tok_t* lex_statement(proj_t* proj) {
    tok_t* tok = lex_identifier(proj);
    if (!tok) return NULL;
    tok_t statement = {0};
    switch (tok->type) {
    case func_tok:
        tok_t* tok = lex_identifier(proj);
        if (!tok||tok->type!=identifier_tok) return NULL;
        statement.file = tok->file;
        statement.ptr  = tok->ptr;
        statement.len  = tok->len;
        statement.type = func_holder;

        tok_t* paren = lex_operator(proj);
        if (!paren||paren->type!=parentheses_open) return NULL;
        
        break;
    
    default:
        break;
    }
    // LOG("%.*s", (int)identifier->len, identifier->ptr);
    // if (identifier->type==type_tok) DEBUG(identifier->type);

    return tok;
}