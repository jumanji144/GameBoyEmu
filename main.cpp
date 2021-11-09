#include "debugger.h"
#include "rom.h"
#include "emulator.h"

int main(int argc, char const *argv[])
{

    Rom* rom = loadRom("../main.gb");

    init(rom);

    init_debugview();

    attach_emulator();

    while(!should_close_debugview()) {

        render_debugview();

    }

    end_debugview();

    return 0;
}
