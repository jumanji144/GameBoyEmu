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
            C = value;
            break;
        case 4:
            E = value;
            break;
        case 5:
            L = value;
            break;
        case 6:
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
            return C;
            break;
        case 4:
            return E;
            break;
        case 5:
            return L;
            break;
        case 6:
            return A;
            break;
    }
    return 0;
}

void inc8(u8 reg) {
    u8 value = read8(reg);
    value++;
    fN = false;
    fZ = value == 0;
    fH = (value & 0xf) == 0;
    write8(reg, value);
}

void dec8(u8 reg) {
    u8 value = read8(reg);
    value--;
    fN = true;
    fZ = value == 0;
    fH = (value & 0xf) == 0xf;
    write8(reg, value);
}

void inc16(u8 reg) {
    write16(reg, read16(reg) + 1);
}

void dec16(u8 reg) {
    write16(reg, read16(reg) - 1);
}

void add16(u16 val) {
    fN = false;
    fC = (HL() + val) > 0xffff;
    fH = ((HL() & 0xfff) + (val & 0xfff)) > 0xfff;
    write16(2, (HL() + val));
}

void add(u8 val) {
    fN = false;
    fZ = (A + val)== 0;
    fH = ((int)(A & 0xf) + (int)(val & 0xf)) > 0xf;
    fC = ((int)(A) + (int)(val)) > 0xff;
    A += val;
}

void adc(u8 val) {
    int c = fC;
    fN = false;
    fZ = (A + val + c)== 0;
    fH = ((int)(A & 0xf) + (int)(val & 0xf) + (int)c) > 0xf;
    fC = ((int)(A) + (int)(val) +(int)(c)) > 0xff;
    A += val + c;
}
/*
- Z: Set if result is 0 (a == b)
- N: 1
- H: Set if no borrow from bit 4
- C: Set if no borrow (a < b)
*/
void sub(u8 val) {
    fN = true;
    fZ = (A - val) == 0;
    fH = ((int)(A & 0xf) - (int)(val & 0xf)) < 0;
    fC = ((int)A - (int)val) < 0;
    A -= val;
}

void sbc(u8 val) {
    int c = fC;
    fN = true;
    fZ = (A - val - c) == 0;
    fH = ((int)(A & 0xf) - (int)(val & 0xf) - (int)c) < 0;
    fC = ((int)A - (int)val - (int)c) < 0;
    A -= val - c;
}

void and(u8 val) {
    u8 value = A & val;
    fZ = value == 0;
    fH = true;
    A = value;
}

void xor(u8 val) {
    u8 value = A ^ val;
    fZ = value == 0;
    A = value;
}

void or(u8 val) {
    u8 value = A | val;
    fZ = value == 0;
    A = value;
}

void cp(u8 val) {
    fN = true;
    fZ = (A - val) == 0;
    fH = ((int)(A & 0xf) - (int)(val & 0xf)) < 0;
    fC = ((int)A - (int)val) < 0;
}

void rl(u8 reg, bool carry) {
    u8 value = read8(reg);
    u8 c = (value >> 7) & 1;
    value = (value << 1) | carry ? c : fC;
    if(carry)
    fC = c;
    write8(reg, value);
}

void rr(u8 reg, bool carry) {
    u8 value = read8(reg);
    u8 c = (value << 7) & 1;
    value = (value >> 1) | carry ? c : fC;
    if(carry)
        fC = c;
    write8(reg, value);
}
