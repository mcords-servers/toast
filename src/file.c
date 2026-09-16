#include "kit.h"

file_t* open_file(char* path) {
    int fd = open(path, O_RDONLY);
    if (fd<0) return NULL;
    size_t flen = lseek(fd, 0, SEEK_END); lseek(fd, 0, 0);
    char* buf = malloc(flen+1); buf[flen]=0; read(fd, buf, flen);

    file_t* file = calloc(1, sizeof(file_t));
    file->buf = buf; file->flen = flen; file->ptr = buf;
    return file;
}

