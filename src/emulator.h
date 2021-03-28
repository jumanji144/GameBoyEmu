#include "include.h"
#include "util.h"
#include "memory.h"
#include <iostream>

#define end PC += 1; break;
#define endu(u) PC += u; break;
#define reg (inst >> 4)
#define jr PC += read8I();
#define ld_r_r(base, R) \
    case base: write8(R, read8(0)); end \
    case base + 1: write8(R, read8(3)); end \
    case base + 2: write8(R, read8(1)); end \
    case base + 3: write8(R, read8(4)); end \
    case base + 4: write8(R, read8(2)); end \
    case base + 5: write8(R, read8(5)); end \
    case base + 6: write8(R, read8Addr(HL())); end \
    case base + 7: write8(R, read8(6)); end
#define op_r_r(base, op) \
    case base: op(read8(0)); end \
    case base + 1: op(read8(3)); end \
    case base + 2: op(read8(1)); end \
    case base + 3: op(read8(4)); end \
    case base + 4: op(read8(2)); end \
    case base + 5: op(read8(5)); end \
    case base + 6: op(read8Addr(HL())); end \
    case base + 7: op(read8(6)); end

Rom* current_Rom;
bool debug = true;

u8 get_inst() {
    return current_Rom->data[PC];
}

u16 read16I() {
    u8 lower = current_Rom->data[++PC];
    u8 upper = current_Rom->data[++PC];
    return ((u16)upper) << 8 | ((u16)lower);
}

u8 read8I() {
    return current_Rom->data[++PC];
}

u8 get_inst_at(u8 pointer) {
    return current_Rom->data[pointer];
}

u8 peek(u8 value) {
    return current_Rom->data[PC + value];
}

u8 load_rom(Rom* rom ) {
    current_Rom = rom;
    return 0;
}

void exec_inst() {
    try{
    u8 inst = get_inst();
    if(debug)
        printf("Instruction: 0x%x\n",inst);
    switch(inst) {
        case 0x00: PC += 1; break;
        case 0x10: exit(1); // TODO: STOP impl
        case 0x01: case 0x11: case 0x21: case 0x31: write16(reg, read16I()); end
        case 0x02: case 0x12: write8Addr(read16(reg), A); end
        case 0x22: write8Addr(HL(), A); inc16(2); end
        case 0x32: write8Addr(HL(), A); inc16(2); end
        case 0x03: case 0x13: case 0x23: case 0x33: inc16(reg); end
        case 0x04: case 0x14: case 0x24: inc8(reg); end
        case 0x34: inc8M(HL()); end
        case 0x05: case 0x15: case 0x25: dec8(reg); end
        case 0x35: dec8M(HL()); end
        case 0x06: case 0x16: case 0x26: write8(reg, read8I()); end
        case 0x36: write8Addr(HL(), read8I()); end
        case 0x07: rl(6, true); end
        case 0x17: rl(6, false); end
        case 0x27: end // TODO: DAA
        case 0x37: fH = false; fN = false; fC = true; end
        case 0x08: write16Addr(read16I(), SP); end
        case 0x18: jr break;
        case 0x28: if(fZ) jr break;
        case 0x38: if(fC) jr break;
        case 0x09: case 0x19: case 0x29: case 0x39: add16(read16(reg)); end
        case 0x0A: case 0x1A: A = read8Addr(read16(reg)); end
        case 0x2A: A = read8Addr(HL()); inc16(2); end
        case 0x3A: A = read8Addr(HL()); inc16(2); end
        case 0x0B: case 0x1B: case 0x2B: case 0x3B: dec16(reg); end
        case 0x0C: case 0x1C: case 0x2C: case 0x3C: inc8(reg + 3); end
        case 0x0D: case 0x1D: case 0x2D: case 0x3D: dec8(reg + 3); end
        case 0x0E: case 0x1E: case 0x2E: case 0x3E: write8(reg + 3, read8I()); end
        case 0x0f: rr(6, true); end
        case 0x1f: rr(6, false); end
        case 0x2f: A = ~A; fH= true; fN= true; end
        case 0x3f: fC = !fC; fH= false; fN= false; end
        ld_r_r(0x40, 0)
        ld_r_r(0x48, 3)
        ld_r_r(0x50, 1)
        ld_r_r(0x58, 4)
        ld_r_r(0x60, 2)
        ld_r_r(0x68, 5)
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: write8Addr(HL(), read8(reg)); end
        case 0x76: printf("[HALT]"); exit(0);
        case 0x77: write8Addr(HL(), A);
        ld_r_r(0x78, 6)
        op_r_r(0x80, add);
        op_r_r(0x88, adc);
        op_r_r(0x90, sub);
        op_r_r(0x98, sbc);
        op_r_r(0xA0, and);
        op_r_r(0xA8, xor);
        op_r_r(0xB0, or);
        op_r_r(0xB8, cp);
        case 0xC3: PC = read16I(); break;
        default:
            printf("[WARN] Unknown instruction at: %d\n", PC);
            PC += 1;
            break;    
    }
    char* flagbit = strdup("0000000");
    if(fZ)
        flagbit[0] = '1';
    if(fN)
        flagbit[1] = '1';
    if(fH)
        flagbit[2] = '1';
    if(fC)
        flagbit[3] = '1';
    if(debug) {
        printf("Registers:\nA: 0x%x, B: 0x%x, C: 0x%x, D: 0x%x, E: 0x%x, F: 0x%x, H: 0x%x, L: 0x%x, AF: 0x%x, BC: 0x%x, DE: 0x%x, HL: 0x%x, PC: 0x%x, SP: 0x%x\nFlag Bit:\n%s\n", A, B, C, D, E, F, H, L, AF(), BC(), DE(), HL(), PC, SP,flagbit);
    }
    delete[] flagbit;
        } catch(...) {
        printf("yes");
    }
}

void execute(Rom* rom, size_t off=0x100) {
    current_Rom = rom;
    PC = off;
    initMem(rom);
    while(1) {
        char c;
        if(debug) 
            std::cin >> c;
        exec_inst();
        if(PC>= rom->total)
            break;
    }
}
