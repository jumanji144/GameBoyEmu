#include "include.h"
#include "registers.h"

typedef struct {
    u8* data;
    u16 offset = 0x1000;
    u8 current_bank = 0;
}RAM;

RAM* WRAM;
Rom* ROM;
bool log_access = false;

void initMem(Rom* rom) {
    WRAM = (RAM*)malloc(sizeof(RAM));
    WRAM->data = (u8*)malloc(0x2000); 
    ROM = rom;
}

#define check(addr, offset) if(addr - offset < 0) \
 { \
   printf("[WARN] MEMORY_ACCESS_ERROR: %d\n", addr); \
   return 0; \
 } 

#define checkW(addr, offset) if(addr - offset < 0) \
 { \
   printf("[WARN] MEMORY_ACCESS_ERROR: %d\n", addr); \
   return; \
 } 

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
    if(log_access)
    printf("[INFO] MEMORY_ACCESS: R: 0x%x (%d)\n", addr, findMemoryRegion(addr));
    switch(findMemoryRegion(addr)) {
        case VRAM:
            break; // impl later
        case ROM0:
            check(addr, 0)
            return ROM->data[max(0, addr)];
        case ROM1: // impl later
            break;
        case RAM0: // Read first 1000 bytes from 
            check(addr, 0xC000)
            return WRAM->data[max(0, addr - 0xC000)]; // put the value offseted into the RAM
        case RAM1:
            check(addr, 0xC000)
            return WRAM->data[max(0, (addr - 0xC000) + WRAM->offset)];
        default:
            return 0;
    }
    return 0;
}

u16 read16Addr(u16 addr) {
    return 0;
}

void write8Addr(u16 addr, u8 value) {
      if(log_access)
    printf("[INFO] MEMORY_ACCESS: W: 0x%x (%d)\n", addr, findMemoryRegion(addr));
    switch(findMemoryRegion(addr)) {
        case VRAM:
            break; // impl later
        case ROM0:
        case ROM1:
            break; // READ ONLY
        case RAM0: // Read first 1000 bytes from 
            checkW(addr, 0xC000)
            WRAM->data[max(0, addr - 0xC000)] = value; // put the value offseted into the RAM
        case RAM1:
            WRAM->data[max(0, (addr - 0xC000) + WRAM->offset)] = value;
    }
    return;
}

void inc8M(u16 addr) {
    u8 value = read8Addr(addr);
    value++;
    fN = false;
    fZ = value == 0;
    fH = (value & 0xf) == 0;
    write8Addr(addr, value);
}

void dec8M(u16 addr) {
    u8 value = read8Addr(addr);
    value--;
    fN = true;
    fZ = value == 0;
    fH = (value & 0xf) == 0xf;
    write8Addr(addr, value);
}

void write16Addr(u16 addr, u16 value) {
}
