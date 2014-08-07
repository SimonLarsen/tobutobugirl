default: game.gb

maps:
	imgtogb --map data/bg/background.png -o data/bg/background.h

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	
game.gb: maps sprites main.c
	lcc main.c -o game.gb

.PHONY: run
run:
	gambatte -s 3 game.gb
