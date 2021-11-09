#pragma once

#include "include.h"
#include "rom.h"

#define MEMORY_SIZE 0xFFFF // 1024 for now

void memory_init();
void write_byte(u16 address, u8 value);
/**
 * @brief Will return the memory pointer
 * 
 * @return u8* 
 */
u8* request_memory();
void load_rom(Rom* rom);
u8 read_byte(u16 address);