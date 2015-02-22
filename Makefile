default: backgrounds sprites tobu.gb

backgrounds:
	imgtogb --map data/bg/powerups.png -o data/bg/powerups.h
	imgtogb --map data/bg/hud.png -o data/bg/hud.h -O 22
	imgtogb --map data/bg/clock.png -o data/bg/clock.h -O 40
	imgtogb --map data/bg/background.png -o data/bg/background.h -O 54
	imgtogb --map data/bg/title.png -o data/bg/title.h
	imgtogb --map data/bg/circles.png -o data/bg/circles.h
	imgtogb --map data/bg/select.png -o data/bg/select.h -O 8

sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h

.PHONY: tobu.gb
tobu.gb: 
	lcc -c main.c 
	lcc -c title.c
	lcc -c select.c
	lcc -c game.c
	lcc -c fade.c
	lcc -c gamestate.c
	lcc main.o title.o select.o game.o fade.o gamestate.o -o $@

.PHONY: run
run:
	gambatte -s 3 tobu.gb

.PHONY: clean
clean:
	rm *.o
