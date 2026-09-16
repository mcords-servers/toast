#ifndef KIT_H
#define KIT_H

#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG(x) _Generic((x), short: LOG(#x ": %hd", (x)), unsigned short: LOG(#x ": %hu", (x)), int: LOG(#x ": %d", (x)), unsigned int: LOG(#x ": %u", (x)), long: LOG(#x ": %ld", (x)), unsigned long: LOG(#x ": %lu", (x)), long long: LOG(#x ": %lld", (x)), unsigned long long: LOG(#x ": %llu", (x)), float: LOG(#x ": %f", (x)), double: LOG(#x ": %lf", (x)), char: LOG(#x ": '%c'", (x)), char *: LOG(#x ": \"%s\"", (x)), const char *: LOG(#x ": \"%s\"", (x)), void *: LOG(#x ": %p", (x)), void **: LOG(#x ": %p", (x)) )
#define u64(x) ((uint64_t)(x))&0xFF, (((uint64_t)(x))>>8)&0xFF, (((uint64_t)(x))>>16)&0xFF, (((uint64_t)(x))>>24)&0xFF, (((uint64_t)(x))>>32)&0xFF, (((uint64_t)(x))>>40)&0xFF, (((uint64_t)(x))>>48)&0xFF, (((uint64_t)(x))>>56)&0xFF
#define u32(x) (x)&0xFF, ((x)>>8)&0xFF, ((x)>>16)&0xFF, ((x)>>24)&0xFF

typedef union reg {
    uint64_t i;
    double f;
} reg_t;
typedef struct env {
    uint8_t* bytecode;
    uint8_t stack[1024];
    // rip, rsp, rflag, rbp
    #define _registries (4+32)
    reg_t regs[_registries];
} env_t;
enum byte_code: uint8_t {
    _noop,
    _add, _sub, _mul, _div, _mod,
    _and, _or, _xor, _not, _shL, _shR,
    _eq, _neq, _lt, _gt, _lte, _gte,
    _jmp, _jif, _jnif,
    _mov, _sys,
    _fetch, _store, _const,
    // _const reg u64
};
enum reg_id: uint8_t {
    _rip, _rsp, _rflag, _rbp,
    _i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11, _i12, _i13, _i14, _i15, _i16,
    _f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16,
};
enum tok_type {
    unexpected_tok, eof_tok,
    identifier_tok, array_tok, string_tok,
    index_tok, call_tok, child_tok,

    max_tok
};
typedef struct tok tok_t;
typedef struct tok {
    char* ptr;
    size_t len;
    enum tok_type type;
    tok_t** tokens;
    struct {
        bool present;
        bool plus_minus;
        bool pre_post;
    } unary;
    bool postfix;
} tok_t;
typedef struct stack_frame {
    char* name;
    enum tok_type type;
    tok_t* tok;
} stack_t;
typedef struct file {size_t flen; char* buf; char* ptr; tok_t tok;} file_t;
typedef struct proj {file_t** files; stack_t** frames;} proj_t;

size_t index_append(void*** arr, void* ptr);
size_t index_remove(void*** arr, size_t index);
size_t index_ptr(void*** arr, void* ptr);
size_t bytes_append(uint8_t** dest, uint8_t* src, size_t n);

file_t* open_file(char* path);

#endif