romname = main

$(romname).gb: $(romname).o
	rgblink -p 0xFF -o ../$(romname).gb -n $(romname).sym  $(romname).o

$(romname).o: $(romname).asm
	rgbasm -o $(romname).o $(romname).asm