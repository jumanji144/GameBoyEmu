#include "include.h"

size_t pc;
size_t offset = 0x100;
Rom* current_Rom;

u8 get_inst() {
    return current_Rom->data[offset + pc];
}

void exec_inst() {
    u8 inst = get_inst();

    switch(inst) {
        case 0x00:
            printf("NOP");
            pc += 1;
            break;
        default:
            pc += 1;
            break;    
    }
}

void execute(Rom* rom) {
    exec_inst(); // should be noop
    exec_inst(); // should jump to the instruction loop;

    while(1) {
        exec_inst();
        if(pc + offset >= rom->total)
            break;
    }
}
