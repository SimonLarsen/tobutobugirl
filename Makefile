CC=lcc
CFLAGS=-Wa-l

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
	imgtogb --map data/bg/win_base.png -o data/bg/win_base.h -O 40
	imgtogb --map data/bg/win1.png -o data/bg/win1.h -O 55
	imgtogb --map data/bg/win2.png -o data/bg/win2.h -O 55
	imgtogb --map data/bg/win3.png -o data/bg/win3.h -O 55

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
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) -c fade.c
	$(CC) $(CFLAGS) -c gamestate.c
	$(CC) $(CFLAGS) -c cos.c
	$(CC) $(CFLAGS) -c circles.c
	$(CC) $(CFLAGS) -c characters.c
	$(CC) $(CFLAGS) -c game.c -Wf-bo1
	$(CC) $(CFLAGS) -c title.c -Wf-bo2
	$(CC) $(CFLAGS) -c select.c -Wf-bo2
	$(CC) $(CFLAGS) -c highscore.c -Wf-bo2
	$(CC) $(CFLAGS) -c arrow.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection0.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection1.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection2.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection3.c -Wf-bo2
	$(CC) $(CFLAGS) -c winscreen.c -Wf-bo3
	$(CC) $(CFLAGS) -c -Wf-ba0 -c ram.c
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo4 -Wl-ya1 *.o -o $@

.PHONY: run
run:
	gambatte -s 3 tobu.gb

.PHONY: clean
clean:
	rm *.o
