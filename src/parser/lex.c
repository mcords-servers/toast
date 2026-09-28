#include <kit.h>

bool lex_spaces(proj_t* proj) {
    file_t* file = (file_t*)index_top((void***)&proj->files_stack);
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

tok_t* lex_identifier(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    tok.file = index_top((void***)&proj->files_stack);
    tok.ptr = tok.file->ptr;
    if (!isalpha(tok.file->ptr[0])&&tok.file->ptr[0]!='_') return NULL;
    while (isalnum(tok.file->ptr[0])||tok.file->ptr[0]=='_') tok.file->ptr++;
    tok.len = tok.file->ptr-tok.ptr;
    tok.type = identifier_tok;

    if (tok.len<=8) {
        size_t val = 0;
        memcpy(&val, tok.ptr, tok.len);
        if (val==s8("func")) print_bits(val);
                

    }

    return allocpy(&tok, sizeof(tok_t));
}

tok_t* lex_statement(proj_t* proj) {
    tok_t* identifier = lex_identifier(proj);

    return NULL;
}