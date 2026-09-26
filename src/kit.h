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
    uint64_t u64;
    uint32_t u32;
    uint16_t u16;
    uint8_t  u8;

    int64_t  i64;
    int32_t  i32;
    int16_t  i16;
    int8_t   i8;

    double   f;
    float    f32;
} reg_t;
#define _registries (4+16+8)
typedef struct env {
    uint8_t* bytecode;
    uint8_t stack[1024];
    // rip, rsp, rflag, rbp
    reg_t regs[_registries];
} env_t;
enum byte_code: uint8_t {
    _noop,
    _add, _sub, _mul, _div, _mod,
    _imul, _idiv, _imod,
    _and, _or, _xor, _not, _shL, _shR,
    _eq, _neq, _lt, _gt, _lte, _gte,
    _jmp, _jif, _jnif,
    _mov, _sys,
    _fetch, _store, _const,
};
enum reg_flags: uint64_t {
    _CF = 1<<0,  // carry flag
    _PF = 1<<1,  // parity flag
    _AF = 1<<2,  // adjust flag (idk)    (won't be used likely)
    _ZF = 1<<3,  // zero flag
    _SF = 1<<4,  // sign flag
    _TF = 1<<5,  // trap flag
    _IF = 1<<6,  // interrupt flag
    _DF = 1<<7,  // direction flag (idk) (won't be used likely)
    _OF = 1<<8,  // overflow flag
};
enum reg_id: uint8_t {
    _rip, _rsp, _rflag, _rbp,
    // sizes are either 8 (default), 4, 2, 1 so it's 4 different options that take 2 bits in total
    // maybe just MAYBE i'll add the 128 bit registries or smth
    _i0,  _i0_4,  _i0_2,  _i0_1,
    _i1,  _i1_4,  _i1_2,  _i1_1,
    _i2,  _i2_4,  _i2_2,  _i2_1,
    _i3,  _i3_4,  _i3_2,  _i3_1,
    _i4,  _i4_4,  _i4_2,  _i4_1,
    _i5,  _i5_4,  _i5_2,  _i5_1,
    _i6,  _i6_4,  _i6_2,  _i6_1,
    _i7,  _i7_4,  _i7_2,  _i7_1,
    _i8,  _i8_4,  _i8_2,  _i8_1,
    _i9,  _i9_4,  _i9_2,  _i9_1,
    _i10, _i10_4, _i10_2, _i10_1,
    _i11, _i11_4, _i11_2, _i11_1,
    _i12, _i12_4, _i12_2, _i12_1,
    _i13, _i13_4, _i13_2, _i13_1,
    _i14, _i14_4, _i14_2, _i14_1,
    _i15, _i15_4, _i15_2, _i15_1,
    _f0,  _f0_4,
    _f1,  _f1_4,
    _f2,  _f2_4,
    _f3,  _f3_4,
    _f4,  _f4_4,
    _f5,  _f5_4,
    _f6,  _f6_4,
    _f7,  _f7_4,
};

size_t index_append(void*** arr, void* ptr);
size_t index_remove(void*** arr, size_t index);
size_t index_ptr(void*** arr, void* ptr);
size_t bytes_append(uint8_t** dest, uint8_t* src, size_t n);
void* allocpy(void* val, size_t size);

#endif