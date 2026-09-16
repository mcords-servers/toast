#include "kit.h"

// Idk why i did this file, maybe i'll need it sometime later

env_t env;

int bytecode_run(env_t* env) {
    if (!env||!env->bytecode) return 1;
    uint8_t* bc = env->bytecode;
    size_t* ip = &env->regs[_rip].i, l = ((size_t*)env->bytecode)[-1];

    for (;*ip<l; ip[0]++) {
        size_t len = l-ip[0]-1;
        uint8_t* ptr = env->bytecode+ip[0]+1;
        switch (bc[ip[0]]) {
            case _noop: continue;
            case _const: {
                if (len<9||ptr[0]>_registries) return 2;
                memcpy(&env->regs[ptr[0]], ptr+1, 8);
                ip[0] += 9;
                break;
            }
            case _mov: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                memcpy(&env->regs[ptr[0]], &env->regs[ptr[1]], 8);
                ip[0] += 2;
                break;
            }
            case _add: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i += env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _sub: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i -= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _mul: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i *= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _div: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                if (!env->regs[ptr[1]].i) return 3;
                env->regs[ptr[0]].i /= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _mod: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                if (!env->regs[ptr[1]].i) return 3;
                env->regs[ptr[0]].i %= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _and: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i &= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _or: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i |= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _xor: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i ^= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _not: {
                if (len<1||ptr[0]>_registries) return 2;
                env->regs[ptr[0]].i = ~env->regs[ptr[0]].i;
                ip[0] += 1;
                break;
            }
            case _shL: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i <<= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }
            case _shR: {
                if (len<2||ptr[0]>_registries||ptr[1]>_registries) return 2;
                env->regs[ptr[0]].i >>= env->regs[ptr[1]].i;
                ip[0] += 2;
                break;
            }

            default: break;
        }
    }

    return 0;
}

__attribute__((constructor))
static void test() {
    env.regs[_rsp].i = (uint64_t)env.stack+sizeof(env.stack);
    uint8_t bc[] = {
        _const, _i0, u64(2),
        _shL, _i0, _i0,
    };
    bytes_append(&env.bytecode, bc, sizeof(bc));

    DEBUG(bytecode_run(&env));
    DEBUG(env.regs[_i0].i);
}