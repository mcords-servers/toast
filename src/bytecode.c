#include "kit.h"

// Idk why i did this file, maybe i'll need it sometime later

env_t env;

uint8_t reg_size(enum reg_id reg) {
    if (reg<=_rbp) return 8;
    if (reg<_f0) return 1<<(3-(reg%4));
    if (reg<_f7_4) return 4*(2-(reg%2));
    return 0;
}

int bytecode_run(env_t* env) {
    if (!env||!env->bytecode) return 1;
    uint8_t* bc = env->bytecode;
    size_t* ip = &env->regs[_rip].u64, l = ((size_t*)env->bytecode)[-1];

    for (;*ip<l; ip[0]++) {
        size_t len = l-ip[0]-1;
        uint8_t* ptr = env->bytecode+ip[0]+1;
        switch (bc[ip[0]]) {
            case _noop: break;
            case _const: {
                if (len<9||reg_size(ptr[0])<8) return 2;
                memcpy(&env->regs[ptr[0]], ptr+1, 8);
                ip[0] += 9;
                break;
            }
            case _mov: {
                if (len<2||!reg_size(ptr[0])||!reg_size(ptr[1])) return 2;
                memcpy(&env->regs[ptr[0]], &env->regs[ptr[1]], 8);
                ip[0] += 2;
                break;
            }
            case _add: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 += env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 += env->regs[ptr[1]].u16; break;
                    case 4: env->regs[ptr[0]].u32 += env->regs[ptr[1]].u32; break;
                    case 8: env->regs[ptr[0]].u64 += env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _sub: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 -= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 -= env->regs[ptr[1]].u16; break;
                    case 4: env->regs[ptr[0]].u32 -= env->regs[ptr[1]].u32; break;
                    case 8: env->regs[ptr[0]].u64 -= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _mul: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 *= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 *= env->regs[ptr[1]].u16; break;
                    case 4: env->regs[ptr[0]].u32 *= env->regs[ptr[1]].u32; break;
                    case 8: env->regs[ptr[0]].u64 *= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _div: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1:
                        if (!env->regs[ptr[1]].u8) return 3;
                        env->regs[ptr[0]].u8 /= env->regs[ptr[1]].u8; break;
                    case 2: 
                        if (!env->regs[ptr[1]].u16) return 3;
                        env->regs[ptr[0]].u16 /= env->regs[ptr[1]].u16; break;
                    case 4: 
                        if (!env->regs[ptr[1]].u32) return 3;
                        env->regs[ptr[0]].u32 /= env->regs[ptr[1]].u32; break;
                    case 8: 
                        if (!env->regs[ptr[1]].u64) return 3;
                        env->regs[ptr[0]].u64 /= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _mod: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: 
                        if (!env->regs[ptr[1]].u8) return 3;
                        env->regs[ptr[0]].u8 %= env->regs[ptr[1]].u8; break;
                    case 2: 
                        if (!env->regs[ptr[1]].u16) return 3;
                        env->regs[ptr[0]].u16 %= env->regs[ptr[1]].u16; break;
                    case 4: 
                        if (!env->regs[ptr[1]].u32) return 3;
                        env->regs[ptr[0]].u32 %= env->regs[ptr[1]].u32; break;
                    case 8: 
                        if (!env->regs[ptr[1]].u64) return 3;
                        env->regs[ptr[0]].u64 %= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _imul: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].i8 *= env->regs[ptr[1]].i8; break;
                    case 2: env->regs[ptr[0]].i16 *= env->regs[ptr[1]].i16; break;
                    case 4: env->regs[ptr[0]].i32 *= env->regs[ptr[1]].i32; break;
                    case 8: env->regs[ptr[0]].i64 *= env->regs[ptr[1]].i64; break;
                }
                ip[0] += 2;
                break;
            }
            case _idiv: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1:
                        if (!env->regs[ptr[1]].i8) return 3;
                        env->regs[ptr[0]].i8 /= env->regs[ptr[1]].i8; break;
                    case 2: 
                        if (!env->regs[ptr[1]].i16) return 3;
                        env->regs[ptr[0]].i16 /= env->regs[ptr[1]].i16; break;
                    case 4: 
                        if (!env->regs[ptr[1]].i32) return 3;
                        env->regs[ptr[0]].i32 /= env->regs[ptr[1]].i32; break;
                    case 8: 
                        if (!env->regs[ptr[1]].i64) return 3;
                        env->regs[ptr[0]].i64 /= env->regs[ptr[1]].i64; break;
                }
                ip[0] += 2;
                break;
            }
            case _imod: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: 
                        if (!env->regs[ptr[1]].i8) return 3;
                        env->regs[ptr[0]].i8 %= env->regs[ptr[1]].i8; break;
                    case 2: 
                        if (!env->regs[ptr[1]].i16) return 3;
                        env->regs[ptr[0]].i16 %= env->regs[ptr[1]].i16; break;
                    case 4: 
                        if (!env->regs[ptr[1]].i32) return 3;
                        env->regs[ptr[0]].i32 %= env->regs[ptr[1]].i32; break;
                    case 8: 
                        if (!env->regs[ptr[1]].i64) return 3;
                        env->regs[ptr[0]].i64 %= env->regs[ptr[1]].i64; break;
                }
                ip[0] += 2;
                break;
            }
            case _and: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 &= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 &= env->regs[ptr[1]].u16; break;
                    case 4: env->regs[ptr[0]].u32 &= env->regs[ptr[1]].u32; break;
                    case 8: env->regs[ptr[0]].u64 &= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _or: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 |= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 |= env->regs[ptr[1]].u16; break;
                    case 4: env->regs[ptr[0]].u32 |= env->regs[ptr[1]].u32; break;
                    case 8: env->regs[ptr[0]].u64 |= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _xor: {
                if (len<2||!reg_size(ptr[0])||reg_size(ptr[0])!=reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 ^= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 ^= env->regs[ptr[1]].u16; break;
                    case 4: env->regs[ptr[0]].u32 ^= env->regs[ptr[1]].u32; break;
                    case 8: env->regs[ptr[0]].u64 ^= env->regs[ptr[1]].u64; break;
                }
                ip[0] += 2;
                break;
            }
            case _not: {
                if (len<1||!reg_size(ptr[0])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 = ~env->regs[ptr[0]].u8; break;
                    case 2: env->regs[ptr[0]].u16 = ~env->regs[ptr[0]].u16; break;
                    case 4: env->regs[ptr[0]].u32 = ~env->regs[ptr[0]].u32; break;
                    case 8: env->regs[ptr[0]].u64 = ~env->regs[ptr[0]].u64; break;
                }
                ip[0] += 1;
                break;
            }
            case _shL: {
                if (len<2||!reg_size(ptr[0])||!reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 <<= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 <<= env->regs[ptr[1]].u8; break;
                    case 4: env->regs[ptr[0]].u32 <<= env->regs[ptr[1]].u8; break;
                    case 8: env->regs[ptr[0]].u64 <<= env->regs[ptr[1]].u8; break;
                }
                ip[0] += 2;
                break;
            }
            case _shR: {
                if (len<2||!reg_size(ptr[0])||!reg_size(ptr[1])) return 2;
                switch (reg_size(ptr[0])) {
                    case 1: env->regs[ptr[0]].u8 >>= env->regs[ptr[1]].u8; break;
                    case 2: env->regs[ptr[0]].u16 >>= env->regs[ptr[1]].u8; break;
                    case 4: env->regs[ptr[0]].u32 >>= env->regs[ptr[1]].u8; break;
                    case 8: env->regs[ptr[0]].u64 >>= env->regs[ptr[1]].u8; break;
                }
                ip[0] += 2;
                break;
            }

            default: break;
        }

        if (env->regs[_rflag].u8&_TF) return 4;
    }

    return 0;
}

__attribute__((constructor))
static void test() {
    env.regs[_rsp].u64 = (uint64_t)env.stack+sizeof(env.stack);
    uint8_t bc[] = {
        _const, _i0, u64(2),
        _shL, _i0, _i0,
    };
    bytes_append(&env.bytecode, bc, sizeof(bc));

    DEBUG(bytecode_run(&env));
    DEBUG(env.regs[_i0].u64);
}