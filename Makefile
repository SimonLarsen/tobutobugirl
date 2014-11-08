default: backgrounds sprites game.gb

backgrounds:
	imgtogb --map data/bg/background.png -o data/bg/background.h
	imgtogb --map data/bg/hud.png -o data/bg/hud.h -O 119

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h

.PHONY: game.gb
game.gb: 
	lcc -c main.c 
	lcc -c game.c
	lcc -c fade.c
	lcc -c input.c
	lcc -c gamestate.c
	lcc main.o game.o fade.o input.o gamestate.o -o $@

.PHONY: run
run:
	gambatte -s 3 game.gb

.PHONY: clean
clean:
	rm *.o
