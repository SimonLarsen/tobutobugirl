default: backgrounds sprites levels game.gb

backgrounds:
	imgtogb --map data/bg/background.png -o data/bg/background.h
	imgtogb --map data/bg/window.png -O 105 -o data/bg/window.h
	imgtogb --map data/bg/title.png -o data/bg/title.h


.PHONY: levels
levels:
	lua parselevels.lua > levels.h

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	
.PHONY: game.gb
game.gb: 
	lcc -c main.c
	lcc -c title.c
	lcc -c game.c
	lcc -c fade.c
	lcc -c input.c
	lcc main.o title.o game.o fade.o input.o -o game.gb

.PHONY: run
run:
	gambatte -s 3 game.gb
