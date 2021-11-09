#include "emulator.h"

Registers regs;
u8* data;

#define end break;
#define endj regs.pc--; break; // end for pc set operations

void run() {

    while(TRUE) {
       step();
    }

}

u8 vm_debug = FALSE;

void debug() {
    vm_debug = TRUE;
}

void print_registers() {

    char* flagbit = strdup("0000000");
    if(regs.f.z)
        flagbit[0] = '1';
    if(regs.f.n)
        flagbit[1] = '1';
    if(regs.f.h)
        flagbit[2] = '1';
    if(regs.f.c)
        flagbit[3] = '1';
    printf("Registers:\nA: 0x%x, B: 0x%x, C: 0x%x, D: 0x%x, E: 0x%x, H: 0x%x, L: 0x%x BC: 0x%x, DE: 0x%x, HL: 0x%x, PC: 0x%x, SP: 0x%x\nFlag Bit:\n%s\n",
         regs.a, regs.b, regs.c, regs.d, regs.e, regs.h, regs.l, regs.bc, regs.de, regs.hl, regs.pc, regs.sp,flagbit);


}
void print_memory() {

    // print the first 30 bytes of memory
    printf("\n");
    for(int i = 0; i < 30; i++) {
        printf("%x", read_byte(i));
    }

}

void init(Rom *r) {

    memory_init();
    load_rom(r);

    data = request_memory(); // rom is at 0x0000 -> 0x7fff

}

Registers get_registers() {
    return regs;
}

void set_pc(u16 newPc) {

    regs.pc = newPc;

}

/**
 * @brief Get a 16 bit imidiate value
 * 
 * @return u16 
 */
u16 d16() {

    u8 lower = data[++regs.pc];
    u8 upper = data[++regs.pc];
    return ((u16)upper) << 8 | ((u16)lower);

}

u8 d8() {
    return data[regs.pc++];
}

u16 a8() {
    return 0xFF00 | d8();
}

u16 a16() {
    return d16();
}

int8_t si8() {
    return data[regs.pc++];
}

/**
 * @brief Let the emulator run one cycle
 * This will take the current instruction from the PC and execute it
 */
void step() {

    // fetch
    uint8_t opcode = data[regs.pc];

    switch (opcode)
    {
    case 0x00:
        break;
    case 0x01: // LD BC, d16
        regs.bc = d16(); end
    case 0x02: // LD (BC), A
        write_byte(regs.bc, regs.a); end
    case 0x03: // INC BC
        regs.bc++; end
    case 0x04: // INC B
        regs.b++; end
    case 0x05: // DEC B
        regs.b--; end
    case 0x3E: // LD A, d8
        regs.a = d8(); end
    case 0xC3:// JP d16
        regs.pc = d16(); endj
    case 0xC7: // RST 0
        regs.sp -= 2;
        write_byte(regs.sp, regs.pc >> 8);
        write_byte(regs.sp + 1, regs.pc & 0xFF);
        regs.pc = 0x00; endj
    case 0xE0: // LDH (a8), A
        write_byte(a8(), regs.a); end
    
    
    default:
        break;
    }

    // if vm_debug is true then print the registers and memory and wait for user input
    if(vm_debug) {
        printf("Current instruction: 0x%x\n", opcode);
        printf("\n");
        printf("Press enter to continue\n");
        getchar();
    }

    regs.pc++;

}
