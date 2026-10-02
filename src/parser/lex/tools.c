#include "kit.h"

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

tok_t lex_previous(proj_t* proj) {
    proj->last_tok.file->ptr = proj->last_tok.ptr;
    return proj->last_tok;
}

tok_t* tokdup(proj_t* proj, tok_t* tok) {
    proj->last_tok=tok[0];
    return allocpy(tok, sizeof(tok_t));
}