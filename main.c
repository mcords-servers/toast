#include "kit.h"

file_t** files;

int main() {
    file_t* file = open_file("main.t");
    index_append((void***)&files, file);
    bool lex_identifier(file_t* file);
    if (!lex_identifier(file)) return 0;
    LOG("%.*s",(int)file->tok.len, file->tok.ptr);

    return 0;
}