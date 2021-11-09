#include "memory.h"

u8* memory;

void memory_init() {
    memory = (u8*)calloc(MEMORY_SIZE, sizeof(u8));
    
}

void load_rom(Rom* rom) {
    for (int i = 0; i < rom->total; i++) {
        memory[i] = rom->data[i];
    }
}

u8* request_memory() {
    return memory;
}

void write_byte(u16 address, u8 value)
{
    // TODO: implement correct memory map
    memory[address] = value;
}

u8 read_byte(u16 address)
{
    return memory[address];
}