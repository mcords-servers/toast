#include "kit.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

// I haven't written this file, its only goal is not efficiency but descriptive output

void* error(proj_t* proj, const char* format, ...) {
    va_list args;
    va_start(args, format);

    file_t* file = proj->last_tok->file;
    const char* path = file->path;

    if (proj->path && strncmp(path, proj->path, strlen(proj->path)) == 0) {
        path += strlen(proj->path);

        if (*path == '/')
            path++;
    }

    // Position of token in the file
    const char* tok = proj->last_tok->ptr;
    size_t tok_len = proj->last_tok->len;

    // Calculate line and column (1-based)
    size_t line = 1;
    size_t column = 1;

    for (const char* p = file->buffer; p < tok; p++) {
        if (*p == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }

    // Print error message
    fprintf(stdout, "%s:%zu:%zu: ", path, line, column);

    // GCC-style red "error:"
    fprintf(stdout, "\033[01;31merror:\033[0m ");

    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");

    // Find start of line
    const char* line_start = tok;

    while (line_start > file->buffer && line_start[-1] != '\n')
        line_start--;

    // Find end of line
    const char* line_end = tok;

    while (*line_end && *line_end != '\n')
        line_end++;

    size_t line_len = line_end - line_start;
    size_t tok_offset = tok - line_start;

    // Get terminal width
    struct winsize ws;

    size_t terminal_width = 80;

    if (ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
        terminal_width = ws.ws_col;

    // Space used by: "LINE | "
    size_t line_digits = snprintf(NULL, 0, "%zu", line);
    size_t prefix_width = line_digits + 3;

    size_t available =
        terminal_width > prefix_width
        ? terminal_width - prefix_width
        : 1;

    // Crop source line around token
    size_t start = 0;
    size_t end = line_len;

    if (line_len > available) {
        size_t room = available > 6 ? available - 6 : 1;

        size_t left = room / 2;
        size_t right = room - left;

        if (tok_offset < left)
            left = tok_offset;

        if (line_len - (tok_offset + tok_len) < right)
            right = line_len - (tok_offset + tok_len);

        start = tok_offset - left;
        end = tok_offset + tok_len + right;

        while ((end - start)
             + (start > 0 ? 3 : 0)
             + (end < line_len ? 3 : 0) > available) {

            if (end - (tok_offset + tok_len) >
                tok_offset - start) {

                if (end > tok_offset + tok_len)
                    end--;
                else
                    start++;

            } else {

                if (start < tok_offset)
                    start++;
                else
                    end--;
            }
        }
    }

    int has_prefix = start > 0;
    int has_suffix = end < line_len;

    // Print source line
    fprintf(stdout, "%*zu | ", (int)line_digits, line);

    if (has_prefix)
        fprintf(stdout, "...");

    // Part before token
    size_t visible_start = tok_offset < start ? start : tok_offset;
    size_t visible_end = tok_offset + tok_len;

    if (visible_end > end)
        visible_end = end;

    if (tok_offset > start) {
        fwrite(line_start + start, 1,
               tok_offset - start, stdout);
    }

    // Token in GCC-style red
    fprintf(stdout, "\033[01;31m");

    if (visible_start < visible_end) {
        fwrite(line_start + visible_start, 1,
               visible_end - visible_start, stdout);
    }

    fprintf(stdout, "\033[0m");

    // Part after token
    if (visible_end < end) {
        fwrite(line_start + visible_end, 1,
               end - visible_end, stdout);
    }

    if (has_suffix)
        fprintf(stdout, "...");

    fprintf(stdout, "\n");

    // Print underline
    fprintf(stdout, "%*s | ", (int)line_digits, "");

    if (has_prefix)
        fprintf(stdout, "   ");

    // Spaces before token
    for (size_t i = start; i < tok_offset; i++) {
        if (line_start[i] == '\t')
            fputc('\t', stdout);
        else
            fputc(' ', stdout);
    }

    // GCC-style bright/bold red underline
    fprintf(stdout, "\033[01;31m");

    for (size_t i = visible_start; i < visible_end; i++)
        fputc('^', stdout);

    fprintf(stdout, "\033[0m\n");

    va_end(args);
    return NULL;
}