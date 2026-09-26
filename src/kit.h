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

#include <bytecode/bytecode.h>

#define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG(x) _Generic((x), short: LOG(#x ": %hd", (x)), unsigned short: LOG(#x ": %hu", (x)), int: LOG(#x ": %d", (x)), unsigned int: LOG(#x ": %u", (x)), long: LOG(#x ": %ld", (x)), unsigned long: LOG(#x ": %lu", (x)), long long: LOG(#x ": %lld", (x)), unsigned long long: LOG(#x ": %llu", (x)), float: LOG(#x ": %f", (x)), double: LOG(#x ": %lf", (x)), char: LOG(#x ": '%c'", (x)), char *: LOG(#x ": \"%s\"", (x)), const char *: LOG(#x ": \"%s\"", (x)), void *: LOG(#x ": %p", (x)), void **: LOG(#x ": %p", (x)) )
#define u64(x) ((uint64_t)(x))&0xFF, (((uint64_t)(x))>>8)&0xFF, (((uint64_t)(x))>>16)&0xFF, (((uint64_t)(x))>>24)&0xFF, (((uint64_t)(x))>>32)&0xFF, (((uint64_t)(x))>>40)&0xFF, (((uint64_t)(x))>>48)&0xFF, (((uint64_t)(x))>>56)&0xFF
#define u32(x) (x)&0xFF, ((x)>>8)&0xFF, ((x)>>16)&0xFF, ((x)>>24)&0xFF

size_t index_append(void*** arr, void* ptr);
size_t index_remove(void*** arr, size_t index);
size_t index_ptr(void*** arr, void* ptr);
size_t bytes_append(uint8_t** dest, uint8_t* src, size_t n);
void* allocpy(void* val, size_t size);

#endif