#include "include.h"
#include "util.h"
#include "registers.h"
#include <iostream>

size_t offset;
Rom* current_Rom;
bool debug = true;

u8 get_inst() {
    return current_Rom->data[offset + PC];
}

u16 read16bit() {

}

u8 get_inst_at(u8 pointer) {
    return current_Rom->data[offset + pointer];
}

u8 peek(u8 value) {
    return current_Rom->data[offset + PC + value];
}

u8 load_rom(Rom* rom ) {
    current_Rom = rom;
}

void exec_inst() {
    try{
    u8 inst = get_inst();
    if(debug)
        printf("Instruction: 0x%x\n",inst);
    switch(inst) {
        case 0x00:
            PC += 1;
            break;
        case 0x01:
        case 0x11:
        case 0x21:
        case 0x31:
            PC += 3;
            break;
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
        } catch(...) {
        printf("yes");
    }
}

void execute(Rom* rom, size_t off=0x100) {
    current_Rom = rom;
    offset = off;

    while(1) {
        char c;
        if(debug) 
            std::cin >> c;
        exec_inst();
        if(PC + offset >= rom->total)
            break;
    }
}
