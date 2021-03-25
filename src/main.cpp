#include "rom.h"
#include "emulator.h"

bool _debug = true;

int main(int argc, char const *argv[])
{
    if(_debug) {
    Rom* rom = (Rom*)malloc(sizeof(Rom));
    rom->data = new u8[4] {0x21, 0xFF, 0xFE, 0x22};
    rom->total = 4;

    execute(rom, 0);
    }else {
        Rom* rom = loadRom(argv[0]);
        execute(rom, 0x100);
    }

    return 0;
}
