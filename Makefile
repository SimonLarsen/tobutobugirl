default: backgrounds sprites tobu.gb

.PHONY: title_backgrounds
title_backgrounds:
	imgtogb --map data/bg/title.png -o data/bg/title.h

.PHONY: select_backgrounds
select_backgrounds:
	imgtogb --map data/bg/select.png -o data/bg/select.h -O 9
	imgtogb --map data/bg/selection1.png -o data/bg/selection1.h -O 40
	imgtogb --map data/bg/selection2.png -o data/bg/selection2.h -O 40
	imgtogb --map data/bg/selection3.png -o data/bg/selection3.h -O 40

.PHONY: ingame_backgrounds
ingame_backgrounds:
	imgtogb --map data/bg/powerups.png -o data/bg/powerups.h
	imgtogb --map data/bg/hud.png -o data/bg/hud.h -O 22
	imgtogb --map data/bg/clock.png -o data/bg/clock.h -O 42
	imgtogb --map data/bg/background1.png -o data/bg/background1.h -O 56
	imgtogb --map data/bg/background2.png -o data/bg/background2.h -O 56
	imgtogb --map data/bg/background3.png -o data/bg/background3.h -O 56

.PHONY: winscreen_backgrounds
winscreen_backgrounds:
	imgtogb --map data/bg/winscreen.png -o data/bg/winscreen.h

.PHONY: highscore_backgrounds
highscore_backgrounds:
	imgtogb --map data/bg/highscore.png -o data/bg/highscore.h -O 46

backgrounds: title_backgrounds select_backgrounds ingame_backgrounds winscreen_backgrounds highscore_backgrounds
	#imgtogb --map data/bg/circles.png -o data/bg/circles.h # 9 tiles

.PHONY: sprites
sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	#imgtogb --sprite data/sprite/characters.png -o data/sprite/characters.h # 37 tiles
	#imgtogb --sprite data/sprite/arrow.png -o data/sprite/arrow.h

.PHONY: tobu.gb
tobu.gb: 
	lcc -c main.c 
	lcc -c title.c
	lcc -c select.c
	lcc -c game.c
	lcc -c winscreen.c
	lcc -c highscore.c
	lcc -c fade.c
	lcc -c gamestate.c
	lcc -c cos.c
	lcc -Wf-ba0 -c bank0.c
	lcc -c data/bg/circles.c -o circles.o
	lcc -c data/sprite/arrow.c -o arrow.o
	lcc -c data/sprite/characters.c -o characters.o
	lcc -Wl-yt3 -Wl-yo2 -Wl-ya4 *.o -o $@
	#lcc -Wl-yt3 -Wl-yo2 -Wl-ya4 main.o title.o select.o winscreen.o highscore.o game.o fade.o gamestate.o cos.o bank0.o -o $@

.PHONY: run
run:
	gambatte -s 3 tobu.gb

.PHONY: clean
clean:
	rm *.o
