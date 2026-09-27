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

    

    return file;
}

proj_t* new_project(char* filename) {
    proj_t* proj = calloc(1, sizeof(proj_t));
    file_t* main = open_file(proj, filename);
    
    return proj;
}