default: backgrounds sprites tobu.gb

backgrounds:
	imgtogb --map data/bg/powerups.png -o data/bg/powerups.h
	imgtogb --map data/bg/hud.png -o data/bg/hud.h -O 20
	imgtogb --map data/bg/background.png -o data/bg/background.h -O 42

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h

.PHONY: tobu.gb
tobu.gb: 
	lcc -c main.c 
	lcc -c game.c
	lcc -c fade.c
	lcc -c input.c
	lcc -c gamestate.c
	lcc main.o game.o fade.o input.o gamestate.o -o $@

.PHONY: run
run:
	gambatte -s 3 tobu.gb

.PHONY: clean
clean:
	rm *.o
