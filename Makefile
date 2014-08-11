default: backgrounds sprites levels game.gb

backgrounds:
	imgtogb --map data/bg/background.png -o data/bg/background.h
	imgtogb --map data/bg/window.png -O 117 -o data/bg/window.h
	imgtogb --map data/bg/title.png -o data/bg/title.h
	imgtogb --map data/bg/level.png -o data/bg/level.h
	imgtogb --map data/bg/leveltext.png -O 53 -o data/bg/leveltext.h

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	imgtogb --sprite data/sprite/numbers.png -o data/sprite/numbers.h
	imgtogb --sprite data/sprite/levelsprites.png -O 10 -o data/sprite/levelsprites.h
	
.PHONY: levels
levels:
	lua parselevels.lua > levels.h

.PHONY: game.gb
game.gb: 
	lcc -c main.c 
	lcc -c title.c
	lcc -c level.c
	lcc -c game.c
	lcc -c fade.c
	lcc -c input.c
	lcc -c gamestate.c
	lcc main.o title.o level.o game.o fade.o input.o gamestate.o -o game.gb

.PHONY: run
run:
	gambatte -s 3 game.gb
