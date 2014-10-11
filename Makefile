default: backgrounds sprites levels game.gb

backgrounds:
	imgtogb --map data/bg/tiles.png -o data/bg/tiles.h
	imgtogb --map data/bg/background.png -O 4 -o data/bg/background.h

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	
.PHONY: levels
levels:
	lua parselevels.lua > levels.h

.PHONY: game.gb
game.gb: 
	lcc -c main.c 
	lcc -c title.c
	lcc -c game.c
	lcc -c fade.c
	lcc -c input.c
	lcc -c gamestate.c
	lcc main.o title.o game.o fade.o input.o gamestate.o -o game.gb

.PHONY: run
run:
	gambatte -s 3 game.gb

.PHONY: clean
clean:
	rm *.o
