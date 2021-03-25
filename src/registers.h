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
        *reg2 = (u8) (*value >> 8);
        *reg1 = (u8) (*value);
    }else {
        return (u16) (*reg2 << 8) | *reg1;
    }
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
