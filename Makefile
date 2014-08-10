default: game.gb

backgrounds:
	imgtogb --map data/bg/background.png -o data/bg/background.h
	imgtogb --map data/bg/window.png -O 105 -o data/bg/window.h


.PHONY: levels
levels:
	lua parselevels.lua > levels.h

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	
game.gb: backgrounds sprites levels game.c
	lcc game.c -o game.gb

.PHONY: run
run:
	gambatte -s 3 game.gb
