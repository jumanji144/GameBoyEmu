#include "include.h"

typedef struct {
    u8 headerOffset[0x100];
    u8 entryPoint[0x4];
    u8 logoBitmap[0x30];
    u8 title[0xB];
    u8 manufactureCode[0x4];
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

RomHeader* readHeader(Rom* rom) {
    return (RomHeader*)rom->data;
}

Rom* loadRom(const char* filename) {
    Rom* rom;
    FILE* f;
    u32 total_size;

    f = fopen(filename, "rb");

    if(f) {
        fseek(f, 0, SEEK_END); // Jump to end of file
        total_size = ftell(f); // Get position and store into total_size because that is the total size of the file

        rom = (Rom*)malloc(sizeof(Rom)); // create new Rom struct
        rom->data = (u8*)malloc(total_size); // pre alloc array
        rom->total = total_size;
        fseek(f, 0, SEEK_SET); // jump back to beginning of file

        if(fread(rom->data, 1, total_size, f) != total_size) { // when read array doesnt match expected size, free the array and quit execution
            
            free(rom);

            fclose(f);

            printf("Error loading rom file %s\n", filename);

            exit(1);

        }

        rom->name = strdup(filename);
    }else {
        printf("Error loading rom file %s\n", filename);
        exit(1);
    }

    fclose(f);

    rom->header = readHeader(rom);

    return rom;
}