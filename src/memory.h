#include "include.h"

typedef struct {
    u8 data[0x2000];
    u16 offset = 0x1000;
    u8 current_bank = 0;
}RAM;

RAM* WRAM = (RAM*)malloc(sizeof(RAM));
Rom* ROM;

enum MEMORY_REGION {
    INTERUPT,
    WORK_RAM,
    HARDWARE_IO,
    UNUSED_RAM,
    OAM,
    ECHO_RAM,
    RAM1,
    RAM0,
    ROM_RAM,
    VRAM,
    ROM1,
    ROM0,
    ROM_HEADER,
    INTERUPT_VECTORS,
    UNKNOWN
};

MEMORY_REGION findMemoryRegion(u16 addr) {
    switch (addr >> 12) { // first 4 bits
        case 0xf: // Interupt flag - OAM
            break;
        case 0xe: // echo ram, block use
            return ECHO_RAM;
        case 0xd: // Internal Banked RAM
            return RAM1;
        case 0xc: // Internal RAM
            return RAM0;
        case 0xA:
        case 0xB: // Cart RAM
            return ROM_RAM;
        case 0x9: case 0x8: 
            return VRAM;
        case 0x4:case 0x5:case 0x6:case 0x7:
            return ROM1;
        case 0x0:case 0x1:case 0x2:case 0x3:
            return ROM0;
        default:
            return UNKNOWN;
    }
    return UNKNOWN;
}

u8 read8Addr(u16 addr) {
        switch(findMemoryRegion(addr)) {
        case VRAM:
            break; // impl later
        case ROM0:
            return ROM->data[addr - 0150];
        case ROM1: // impl later
            break;
        case RAM0: // Read first 1000 bytes from 
            return WRAM->data[addr - 0xC000]; // put the value offseted into the RAM
        case RAM1:
            return WRAM->data[(addr - 0xC000) + WRAM->offset];
        default:
            return UNKNOWN;
    }
    return UNKNOWN;
}

u16 read16Addr(u16 addr) {
    return 0;
}

void write8Addr(u16 addr, u8 value) {
    switch(findMemoryRegion(addr)) {
        case VRAM:
            break; // impl later
        case ROM0:
        case ROM1:
            break; // READ ONLY
        case RAM0: // Read first 1000 bytes from 
            WRAM->data[addr - 0xC000] = value; // put the value offseted into the RAM
        case RAM1:
            WRAM->data[(addr - 0xC000) + WRAM->offset] = value;
    }
    return;
}

void write16Addr(u16 addr, u16 value) {
}
