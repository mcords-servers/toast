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

tok_t* lex_statement(proj_t* proj) {

    return NULL;
}