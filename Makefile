CC=lcc
CFLAGS=-Wa-l

default: backgrounds sprites tobu.gb

.PHONY: title_backgrounds
title_backgrounds:
	imgtogb --map --rle data/bg/titlescreen.png data/bg/titlescreen.h

.PHONY: select_backgrounds
select_backgrounds:
	imgtogb --map -O 9 data/bg/select.png data/bg/select.h

.PHONY: ingame_backgrounds
ingame_backgrounds:
	imgtogb --map data/bg/hud.png data/bg/hud.h
	imgtogb --map -O 12 data/bg/clock.png data/bg/clock.h 
	imgtogb --map -O 26 --rle data/bg/background1.png data/bg/background1.h 
	imgtogb --map -O 26 --rle data/bg/background2.png data/bg/background2.h 
	imgtogb --map -O 26 --rle data/bg/background3.png data/bg/background3.h 

.PHONY: winscreen_backgrounds
winscreen_backgrounds:
	imgtogb --map -O 40 data/bg/win_base.png data/bg/win_base.h
	imgtogb --map -O 55 --rle data/bg/win1.png data/bg/win1.h
	imgtogb --map -O 55 --rle data/bg/win2.png data/bg/win2.h
	imgtogb --map -O 55 --rle data/bg/win3.png data/bg/win3.h

.PHONY: highscore_backgrounds
highscore_backgrounds:
	imgtogb --map -O 109 data/bg/highscore.png data/bg/highscore.h

backgrounds: title_backgrounds select_backgrounds ingame_backgrounds winscreen_backgrounds highscore_backgrounds
	#imgtogb --map data/bg/circles.png data/bg/circles.h # 9 tiles

.PHONY: sprites
sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png data/sprite/sprites.h
	#imgtogb --sprite data/sprite/characters.png data/sprite/characters.h # 40 tiles
	#imgtogb --sprite data/sprite/arrow.png data/sprite/arrow.h
	#imgtogb --map -O 40 data/bg/selection0.png data/bg/selection0.h
	#imgtogb --map -O 40 data/bg/selection1.png data/bg/selection1.h # 54 tiles
	#imgtogb --map -O 40 data/bg/selection2.png data/bg/selection2.h # 62 tiles
	#imgtogb --map -O 40 data/bg/selection3.png data/bg/selection3.h # 40 tiles

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
