#include <kit.h>

file_t* open_file(proj_t* proj, char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd==-1) return NULL;
    size_t len = lseek(fd, 0, SEEK_END); lseek(fd, 0, SEEK_SET);
    char* buffer = malloc(len+1);
    if (read(fd, buffer, len) < len || !buffer) {
        free(buffer);
        return NULL;
    } buffer[len] = '\0';

    file_t* file = calloc(1, sizeof(file_t));
    file->buffer = buffer;
    file->ptr = buffer;
    file->len = len;
    file->path = realpath(filename, NULL);

    index_append((void***)&proj->files, file);
    index_append((void***)&proj->files_stack, file);

    tok_t* statement;
    while ((statement = lex_statement(proj))) {
        index_append((void***)&file->globals, statement);
    }

    return file;
}

inline file_t* top_file(proj_t* proj) {
    return index_top((void***)&proj->files_stack);
}

proj_t* new_project(char* filename) {
    proj_t* proj = calloc(1, sizeof(proj_t));
    proj->path = getcwd(NULL, 0);

    if (setjmp(proj->jump_buffer)) { // Do on error
        
        return proj;
    }
    file_t* main = open_file(proj, filename);

    tok_t** globals = main->globals, *tok;
    size_t i = globals?(size_t)globals[0]+1:0;
    for (; i; i--) {
        // DEBUG(i);
        tok = globals[i+1];
        if (tok->type!=func_holder) continue;
        LOG("function observed");
    }
    
    return proj;
}