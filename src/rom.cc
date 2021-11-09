#include "rom.h"

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

        int read = fread(rom->data, 1, total_size, f); // read the file into the array

        if(read != total_size) { // when read array doesnt match expected size, free the array and quit execution
            
            free(rom);

            fclose(f);

            printf("Error loading rom file %s\nExpected size: %dk got %dk", filename, total_size/1024, read/1024);

            exit(1);

        }

        rom->name = strdup(filename);
    }else {
        printf("Error loading rom file %s\nFile does not exist", filename);
        exit(1);
    }

    fclose(f);

    rom->header = readHeader(rom);

    return rom;
}

void info(Rom *rom) {

    printf("\nName: %s\nSize: %dk", rom->name, rom->total / 1024);
    // print header info
    printf("\n\nHeader:");
    printf("\nTitle: %s", rom->header->title);
    printf("\nCGB: %d", rom->header->cgbFlag);
    printf("\nNew License: %x%x", rom->header->newLicenseCode[0], rom->header->newLicenseCode[1]);
    printf("\nSGB: %d", rom->header->sgbFlag);
    printf("\nCartridge Type: %d", rom->header->type);
    printf("\nROM Size: %d", rom->header->romSize);
    printf("\nRAM Size: %d", rom->header->ramSize);
    printf("\nJapanse?: %s", rom->header->japaneseVersion == 1 ? "Yes" : "No");
    printf("\nOld License: %d", rom->header->oldLicenseCode);
    printf("\nVersion: %d", rom->header->version);
    printf("\nHeader Checksum: %d\n", rom->header->headerChecksum);


}