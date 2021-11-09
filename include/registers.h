#pragma once

#include "include.h"

#define REG_PAIR(X, Y) \
    union { \
        struct { \
            u8 X; \
            u8 Y; \
        }; \
        uint16_t X##Y; \
    }

typedef struct {

    u8 a;
    REG_PAIR(b, c);
    REG_PAIR(d, e);
    REG_PAIR(h, l);
    u16 sp;
    u16 pc;
    struct {
        Bool z, n, h, c;
    } f;

} Registers;

// load macro
#define LR(reg, val) \
    reg = val
