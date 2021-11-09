#pragma once

#include "include.h"

typedef struct {
    u8 headerOffset[0x100];
    u8 entryPoint[0x4];
    u8 logoBitmap[0x30];
    u8 title[0xB];
    u8 cgbFlag;
    u8 newLicenseCode[2];
    u8 sgbFlag;
    u8 type;
    u8 romSize;
    u8 ramSize;
    u8 japaneseVersion;
    u8 oldLicenseCode;
    u8 version;
    u8 headerChecksum;
} RomHeader;

typedef struct {
    char* name;
    u8* data;
    u32 total;
    RomHeader* header;
} Rom;

RomHeader* readHeader(Rom* rom);
Rom* loadRom(const char* filename);

void info(Rom *rom);