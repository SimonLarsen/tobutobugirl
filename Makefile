CC=lcc
CFLAGS=

default: backgrounds sprites tobu.gb

.PHONY: intro_backgrounds
intro_backgrounds:
	imgtogb --map --rle data/bg/intro_bg.png data/bg/intro_bg.h
	imgtogb --sprite --8x16 data/sprite/intro_sprites.png data/sprite/intro_sprites.h
	imgtogb --sprite --8x16 data/sprite/intro_flash.png data/sprite/intro_flash.h

.PHONY: title_backgrounds
title_backgrounds:
	imgtogb --map data/bg/titlescreen.png data/bg/titlescreen.h

.PHONY: select_backgrounds
select_backgrounds:
	imgtogb --map -O 9 data/bg/select.png data/bg/select.h

.PHONY: ingame_backgrounds
ingame_backgrounds:
	imgtogb --map data/bg/hud.png data/bg/hud.h
	imgtogb --map -O 20 data/bg/clock.png data/bg/clock.h 
	imgtogb --map -O 34 --rle data/bg/background1.png data/bg/background1.h 
	imgtogb --map -O 34 --rle data/bg/background2.png data/bg/background2.h 
	imgtogb --map -O 34 --rle data/bg/background3.png data/bg/background3.h 

.PHONY: winscreen_backgrounds
winscreen_backgrounds:
	imgtogb --map -O 40 --rle data/bg/win_base.png data/bg/win_base.h
	imgtogb --map -O 54 --rle data/bg/win1.png data/bg/win1.h
	imgtogb --map -O 54 --rle data/bg/win2.png data/bg/win2.h
	imgtogb --map -O 54 --rle data/bg/win3.png data/bg/win3.h

.PHONY: highscore_backgrounds
highscore_backgrounds:
	imgtogb --map -O 109 data/bg/highscore.png data/bg/highscore.h
	imgtogb --sprite data/sprite/empty.png data/sprite/empty.h

backgrounds: intro_backgrounds title_backgrounds select_backgrounds ingame_backgrounds winscreen_backgrounds highscore_backgrounds

.PHONY: sprites
sprites:
	imgtogb --sprite --8x16 data/sprite/sprites.png data/sprite/sprites.h
	imgtogb --sprite --8x16 data/sprite/portal.png data/sprite/portal.h
	#imgtogb --sprite data/sprite/characters.png data/sprite/characters.h # 40 tiles
	#imgtogb --sprite data/sprite/arrow.png data/sprite/arrow.h
	#imgtogb --map -O 40 data/bg/selection0.png data/bg/selection0.h
	#imgtogb --map -O 40 data/bg/selection1.png data/bg/selection1.h
	#imgtogb --map -O 40 data/bg/selection2.png data/bg/selection2.h
	#imgtogb --map -O 40 data/bg/selection3.png data/bg/selection3.h
	#imgtogb --map -O 40 data/bg/selection4.png data/bg/selection4.h

.PHONY: songs
songs:
	java -jar jar/MMLGB.jar mml/intro_cutscene_part_2.mml data/songs/title_song.asm 4
	java -jar jar/MMLGB.jar mml/mainmenu.mml data/songs/mainmenu_song.asm 4
	java -jar jar/MMLGB.jar mml/score_tally.mml data/songs/winscreen_song.asm 4
	java -jar jar/MMLGB.jar mml/score.mml data/songs/highscore_song.asm 4
	java -jar jar/MMLGB.jar mml/plains.mml data/songs/plains_song.asm 5
	java -jar jar/MMLGB.jar mml/clouds.mml data/songs/clouds_song.asm 5
	java -jar jar/MMLGB.jar mml/space.mml data/songs/space_song.asm 5
	java -jar jar/MMLGB.jar mml/intro_cutscene_part_1.mml data/songs/intro1_song.asm 6
	java -jar jar/MMLGB.jar mml/intro_cutscene_part_2.mml data/songs/intro2_song.asm 6

.PHONY: tobu.gb
tobu.gb: 
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) -c fade.c
	$(CC) $(CFLAGS) -c gamestate.c
	$(CC) $(CFLAGS) -c cos.c
	$(CC) $(CFLAGS) -c circles.c
	$(CC) $(CFLAGS) -c characters.c
	$(CC) $(CFLAGS) -c music.c
	$(CC) $(CFLAGS) -c sound.c
	$(CC) $(CFLAGS) -c freq.c
	$(CC) $(CFLAGS) -c noisefreq.c
	$(CC) $(CFLAGS) -c game.c -Wf-bo1
	$(CC) $(CFLAGS) -c title.c -Wf-bo3
	$(CC) $(CFLAGS) -c intro.c -Wf-bo6
	$(CC) $(CFLAGS) -c select.c -Wf-bo2
	$(CC) $(CFLAGS) -c highscore.c -Wf-bo2
	$(CC) $(CFLAGS) -c arrow.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection0.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection1.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection2.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection3.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection4.c -Wf-bo2
	$(CC) $(CFLAGS) -c winscreen.c -Wf-bo3
	$(CC) $(CFLAGS) -c data/songs/title_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/mainmenu_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/winscreen_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/highscore_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/plains_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/clouds_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/space_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/intro1_song.asm # bank 6
	$(CC) $(CFLAGS) -c data/songs/intro2_song.asm # bank 6
	$(CC) $(CFLAGS) -c -Wf-ba0 -c ram.c
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo8 -Wl-ya1 *.o -o $@

.PHONY: run
run:
	gambatte-qt tobu.gb

.PHONY: clean
clean:
	rm *.o
