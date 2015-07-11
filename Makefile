default: backgrounds sprites tobu.gb

.PHONY: title_backgrounds
title_backgrounds:
	imgtogb --map data/bg/titlescreen.png -o data/bg/titlescreen.h

.PHONY: select_backgrounds
select_backgrounds:
	imgtogb --map data/bg/select.png -o data/bg/select.h -O 9

.PHONY: ingame_backgrounds
ingame_backgrounds:
	imgtogb --map data/bg/hud.png -o data/bg/hud.h -O 0
	imgtogb --map data/bg/clock.png -o data/bg/clock.h -O 12
	imgtogb --map data/bg/background1.png -o data/bg/background1.h -O 26
	imgtogb --map data/bg/background2.png -o data/bg/background2.h -O 26
	imgtogb --map data/bg/background3.png -o data/bg/background3.h -O 26

.PHONY: winscreen_backgrounds
winscreen_backgrounds:
	#imgtogb --map data/bg/win1.png -o data/bg/win1.h -O 40
	#imgtogb --map data/bg/win2.png -o data/bg/win2.h -O 40
	#imgtogb --map data/bg/win3.png -o data/bg/win3.h -O 40

.PHONY: highscore_backgrounds
highscore_backgrounds:
	imgtogb --map data/bg/highscore.png -o data/bg/highscore.h -O 109

backgrounds: title_backgrounds select_backgrounds ingame_backgrounds winscreen_backgrounds highscore_backgrounds
	#imgtogb --map data/bg/circles.png -o data/bg/circles.h # 9 tiles

.PHONY: sprites
sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png -o data/sprite/sprites.h
	#imgtogb --sprite data/sprite/characters.png -o data/sprite/characters.h # 40 tiles
	#imgtogb --sprite data/sprite/arrow.png -o data/sprite/arrow.h
	#imgtogb --map data/bg/selection0.png -o data/bg/selection0.h -O 40
	#imgtogb --map data/bg/selection1.png -o data/bg/selection1.h -O 40 # 54 tiles
	#imgtogb --map data/bg/selection2.png -o data/bg/selection2.h -O 40 # 62 tiles
	#imgtogb --map data/bg/selection3.png -o data/bg/selection3.h -O 40 # 40 tiles

.PHONY: tobu.gb
tobu.gb: 
	lcc -c main.c
	lcc -c title.c
	lcc -c cos.c
	lcc -c fade.c
	lcc -c game.c
	lcc -c gamestate.c
	lcc -c highscore.c
	lcc -c ram.c
	lcc -c select.c
	lcc -c winscreen.c
	lcc -c circles.c
	lcc -c selection0.c
	lcc -c selection1.c
	lcc -c selection2.c
	lcc -c selection3.c
	#lcc -c data/bg/win1.c -Wf-bo2
	#lcc -c data/bg/win2.c -Wf-bo2
	#lcc -c data/bg/win3.c -Wf-bo2
	lcc -c arrow.c
	lcc -c characters.c
	lcc -c screen.c
	lcc -Wf-ba0 -c ram.c
	lcc -Wl-yt1 -Wl-yo4 -Wl-ya1 *.o -o $@

.PHONY: run
run:
	gambatte -s 3 tobu.gb

.PHONY: clean
clean:
	rm *.o
