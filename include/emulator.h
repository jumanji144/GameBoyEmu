#pragma once
#include "registers.h"
#include "rom.h"
#include "memory.h"

void run();
void init(Rom* rom);

// debug functions
void debug();
/**
 * @brief Get (a copy) of the registers of the cpu
 * 
 * @return Registers 
 */
Registers get_registers();

void print_registers();
void print_memory();
void print_rom();

void set_pc(u16 newPc);

/**
 * @brief load 16 bit value from the rom and increment the program counter
 *        first byte is lower, and the second byte is higher
 *        rom->data is a byte array
 * @return u16 
 */
u16 i16();

u8 i8();

/**
 * @brief Let the emulator run one cycle
 * This will take the current instruction from the PC and execute it
 */
void step();

