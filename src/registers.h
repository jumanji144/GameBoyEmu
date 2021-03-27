#include "include.h"

u8 A;
u8 B;
u8 C;
u8 D;
u8 E;
u8 F;
u8 H;
u8 L;
u16 PC;
u16 SP;
bool fZ;
bool fN;
bool fH;
bool fC;

u16 combine(u8 *reg1, u8 *reg2, u16 *value) {
    if(value) {
        *reg1 = (u8) (*value >> 8);
        *reg2 = (u8) (*value);
    }else {
        return (u16) (*reg1 << 8) | *reg2;
    }
    return 0;
}

u16 AF(u16 *value=NULL) { 
    return combine(&A, &F, value);
}

u16 BC(u16 *value=NULL) { 
    return combine(&B, &C, value);
}

u16 DE(u16 *value=NULL) { 
    return combine(&D, &E, value);
}

u16 HL(u16 *value=NULL) { 
    return combine(&H, &L, value);
}

void write16(u8 reg, u16 value) {
    switch(reg) {
        case 0:
            B = (u8) (value >> 8);
            C = (u8)value;
            break;
        case 1:
            D = (u8) (value >> 8);
            E = (u8)value;
            break;
        case 2:
            H = (u8) (value >> 8);
            L = (u8)value;
            break;
        case 3:
            SP = value;
            break;
    }
}

void write16(u8 reg, u8 lower, u8 upper) {
    write16(reg, ((u16)lower) << 8 | ((u16)upper)); 
}

void write8(u8 reg, u8 value) {
    switch(reg) {
        case 0:
            B = value;
            break;
        case 1:
            D = value;
            break;
        case 2:
            H = value;
            break;
        case 3:
            A = value;
            break;
    }
}

u16 read16(u8 reg) {
    switch(reg) {
        case 0:
            return BC();
            break;
        case 1:
            return DE();
            break;
        case 2:
            return HL();
            break;
        case 3:
            return SP;
            break;
    }
    return 0;
}

u8 read8(u8 reg) {
    switch(reg) {
        case 0:
            return B;
            break;
        case 1:
            return D;
            break;
        case 2:
            return H;
            break;
        case 3:
            return A;
            break;
    }
    return 0;
}
