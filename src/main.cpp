#include "rom.h"
#include "emulator.h"

bool _debug = true;

int main(int argc, char const *argv[])
{
    if(argc == 1) {
    Rom* rom = (Rom*)malloc(sizeof(Rom));
    rom->data = new u8[3] {0x22, 0xFF, 0xFE};
    rom->total = 3;

    execute(rom, 0);
    }else {
        Rom* rom = loadRom(argv[1]);

        execute(rom);
    }

    return 0;
}