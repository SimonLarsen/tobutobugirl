CC=lcc
CFLAGS=
IMGTOGB=imgtogb
EMULATOR=gambatte-qt

default: backgrounds sprites tobu.gb

backgrounds: logos_backgrounds intro_backgrounds title_backgrounds select_backgrounds ingame_backgrounds pause_backgrounds winscreen_backgrounds highscore_backgrounds unlocked_backgrounds jukebox_backgrounds

.PHONY: logos_backgrounds
logos_backgrounds:
	$(IMGTOGB) --map --rle data/bg/tangram.png data/bg/tangram.h
	$(IMGTOGB) --map --rle data/bg/potato.png data/bg/potato.h

.PHONY: intro_backgrounds
intro_backgrounds:
	#$(IMGTOGB) --map --rle data/bg/intro_bg.png data/bg/intro_bg.h

.PHONY: title_backgrounds
title_backgrounds:
	$(IMGTOGB) --map data/bg/titlescreen.png data/bg/titlescreen.h

.PHONY: select_backgrounds
select_backgrounds:
	$(IMGTOGB) --map -O 9 data/bg/catface.png data/bg/catface.h
	$(IMGTOGB) --map --rle -O 13 data/bg/select.png data/bg/select.h

.PHONY: ingame_backgrounds
ingame_backgrounds:
	$(IMGTOGB) --map data/bg/hud.png data/bg/hud.h
	$(IMGTOGB) --map -O 20 data/bg/clock.png data/bg/clock.h
	$(IMGTOGB) --map -O 34 --rle data/bg/background1.png data/bg/background1.h
	$(IMGTOGB) --map -O 34 --rle data/bg/background2.png data/bg/background2.h
	$(IMGTOGB) --map -O 34 --rle data/bg/background3.png data/bg/background3.h
	$(IMGTOGB) --map -O 34 --rle data/bg/background4.png data/bg/background4.h

.PHONY: pause_backgrounds
pause_backgrounds:
	$(IMGTOGB) --map --rle -O 38 data/bg/pause_bg.png data/bg/pause_bg.h

.PHONY: winscreen_backgrounds
winscreen_backgrounds:
	$(IMGTOGB) --map -O 40 --rle data/bg/win_base.png data/bg/win_base.h
	$(IMGTOGB) --map -O 69 --rle data/bg/win1.png data/bg/win1.h
	$(IMGTOGB) --map -O 69 --rle data/bg/win2.png data/bg/win2.h
	$(IMGTOGB) --map -O 69 --rle data/bg/win3.png data/bg/win3.h
	$(IMGTOGB) --map -O 69 --rle data/bg/win4.png data/bg/win4.h

.PHONY: highscore_backgrounds
highscore_backgrounds:
	$(IMGTOGB) --map -O 47 data/bg/highscore.png data/bg/highscore.h
	$(IMGTOGB) --sprite data/sprite/empty.png data/sprite/empty.h

.PHONY: unlocked_backgrounds
unlocked_backgrounds:
	$(IMGTOGB) --map -O 47 data/bg/unlocked.png data/bg/unlocked.h
	$(IMGTOGB) --map data/bg/zoom_circles.png data/bg/zoom_circles.h

.PHONY: jukebox_backgrounds
jukebox_backgrounds:
	$(IMGTOGB) --map --rle -O 37 data/bg/jukebox.png data/bg/jukebox.h

.PHONY: sprites
sprites:
	$(IMGTOGB) --sprite --8x16 data/sprite/sprites.png data/sprite/sprites.h
	$(IMGTOGB) --sprite --8x16 data/sprite/portal.png data/sprite/portal.h
	$(IMGTOGB) --sprite --8x16 data/sprite/skin1.png data/sprite/skin1.h
	$(IMGTOGB) --sprite --8x16 data/sprite/skin2.png data/sprite/skin2.h
	$(IMGTOGB) --sprite --8x16 data/sprite/notes.png data/sprite/notes.h
	$(IMGTOGB) --sprite --8x16 data/sprite/intro_sprites.png data/sprite/intro_sprites.h
	$(IMGTOGB) --sprite --8x16 data/sprite/intro_flash.png data/sprite/intro_flash.h
	$(IMGTOGB) --sprite --8x16 data/sprite/bobblehead.png data/sprite/bobblehead.h
	$(IMGTOGB) --sprite --8x16 data/sprite/ending_sprites1.png data/sprite/ending_sprites1.h
	$(IMGTOGB) --sprite --8x16 data/sprite/ending_sprites2.png data/sprite/ending_sprites2.h
	$(IMGTOGB) --sprite data/sprite/digital.png data/sprite/digital.h
	$(IMGTOGB) --sprite --8x16 data/sprite/select_arrow.png data/sprite/select_arrow.h
	$(IMGTOGB) --sprite data/sprite/togglecat.png data/sprite/togglecat.h
	$(IMGTOGB) --sprite --8x16 data/sprite/pause_cloud1.png data/sprite/pause_cloud1.h
	$(IMGTOGB) --sprite --8x16 data/sprite/pause_cloud2.png data/sprite/pause_cloud2.h
	$(IMGTOGB) --sprite --8x16 data/sprite/shine.png data/sprite/shine.h
	$(IMGTOGB) --sprite data/sprite/title_cat.png data/sprite/title_cat.h
	#$(IMGTOGB) --sprite data/sprite/characters.png data/sprite/characters.h # 40 tiles
	#$(IMGTOGB) --sprite --8x16 data/sprite/arrow.png data/sprite/arrow.h
	#$(IMGTOGB) --map -O 90 data/bg/selection1.png data/bg/selection1.h
	#$(IMGTOGB) --map -O 90 data/bg/selection2.png data/bg/selection2.h
	#$(IMGTOGB) --map -O 90 data/bg/selection3.png data/bg/selection3.h
	#$(IMGTOGB) --map -O 90 data/bg/selection4.png data/bg/selection4.h
	#$(IMGTOGB) --map -O 90 data/bg/selection_highscore.png data/bg/selection_highscore.h
	#$(IMGTOGB) --map -O 90 data/bg/selection_jukebox.png data/bg/selection_jukebox.h
	#$(IMGTOGB) --map -O 90 data/bg/selection_locked.png data/bg/selection_locked.h

.PHONY: songs
songs:
	java -jar jar/MMLGB.jar data/mml/intro_cutscene_part_2.mml data/songs/title_song.asm 4
	java -jar jar/MMLGB.jar data/mml/mainmenu.mml data/songs/mainmenu_song.asm 4
	java -jar jar/MMLGB.jar data/mml/score_tally.mml data/songs/winscreen_song.asm 4
	java -jar jar/MMLGB.jar data/mml/score.mml data/songs/highscore_song.asm 4
	java -jar jar/MMLGB.jar data/mml/plains.mml data/songs/plains_song.asm 5
	java -jar jar/MMLGB.jar data/mml/clouds.mml data/songs/clouds_song.asm 5
	java -jar jar/MMLGB.jar data/mml/space.mml data/songs/space_song.asm 5
	java -jar jar/MMLGB.jar data/mml/dream.mml data/songs/dream_song.asm 5
	java -jar jar/MMLGB.jar data/mml/intro_cutscene_part_1.mml data/songs/intro1_song.asm 6
	java -jar jar/MMLGB.jar data/mml/ending_cutscene_part_1.mml data/songs/ending_song.asm 9
	java -jar jar/MMLGB.jar data/mml/potato_jingle.mml data/songs/potato_jingle.asm 9
	java -jar jar/MMLGB.jar data/mml/tangram_shine.mml data/songs/tangram_shine.asm 9
	java -jar jar/MMLGB.jar data/mml/level_clear.mml data/songs/level_clear_song.asm 9

.PHONY: sounds
sounds:
	java -jar jar/MMLGB.jar data/sounds/sfx_bump.mml data/sounds/sfx_bump.h
	java -jar jar/MMLGB.jar data/sounds/sfx_bump_alien.mml data/sounds/sfx_bump_alien.h
	java -jar jar/MMLGB.jar data/sounds/sfx_cat_disable.mml data/sounds/sfx_cat_disable.h
	java -jar jar/MMLGB.jar data/sounds/sfx_cat_enable.mml data/sounds/sfx_cat_enable.h
	java -jar jar/MMLGB.jar data/sounds/sfx_dash.mml data/sounds/sfx_dash.h
	java -jar jar/MMLGB.jar data/sounds/sfx_highscore_switch.mml data/sounds/sfx_highscore_switch.h
	java -jar jar/MMLGB.jar data/sounds/sfx_jetpack.mml data/sounds/sfx_jetpack.h
	java -jar jar/MMLGB.jar data/sounds/sfx_menu_cancel.mml data/sounds/sfx_menu_cancel.h
	java -jar jar/MMLGB.jar data/sounds/sfx_menu_confirm.mml data/sounds/sfx_menu_confirm.h
	java -jar jar/MMLGB.jar data/sounds/sfx_menu_locked.mml data/sounds/sfx_menu_locked.h
	java -jar jar/MMLGB.jar data/sounds/sfx_menu_switch.mml data/sounds/sfx_menu_switch.h
	java -jar jar/MMLGB.jar data/sounds/sfx_player_die.mml data/sounds/sfx_player_die.h
	java -jar jar/MMLGB.jar data/sounds/sfx_stomp_alien.mml data/sounds/sfx_stomp_alien.h
	java -jar jar/MMLGB.jar data/sounds/sfx_stomp_bat.mml data/sounds/sfx_stomp_bat.h
	java -jar jar/MMLGB.jar data/sounds/sfx_stomp_bird.mml data/sounds/sfx_stomp_bird.h
	java -jar jar/MMLGB.jar data/sounds/sfx_stomp_ghost.mml data/sounds/sfx_stomp_ghost.h
	java -jar jar/MMLGB.jar data/sounds/sfx_time_low.mml data/sounds/sfx_time_low.h
	java -jar jar/MMLGB.jar data/sounds/sfx_time_pickup.mml data/sounds/sfx_time_pickup.h
	java -jar jar/MMLGB.jar data/sounds/sfx_warp_end.mml data/sounds/sfx_warp_end.h
	java -jar jar/MMLGB.jar data/sounds/sfx_warp_start.mml data/sounds/sfx_warp_start.h

.PHONY: tobu.gb
tobu.gb:
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) -c fade.c
	$(CC) $(CFLAGS) -c gamestate.c
	$(CC) $(CFLAGS) -c cos.c
	$(CC) $(CFLAGS) -c circles.c
	$(CC) $(CFLAGS) -c characters.c
	$(CC) $(CFLAGS) -c sound.c
	$(CC) $(CFLAGS) -c mmlgb/driver/music.c
	$(CC) $(CFLAGS) -c mmlgb/driver/freq.c
	$(CC) $(CFLAGS) -c mmlgb/driver/noisefreq.c
	$(CC) $(CFLAGS) -c mmlgb/driver/vib.c
	$(CC) $(CFLAGS) -c arrow.c
	$(CC) $(CFLAGS) -c game.c -Wf-bo1
	$(CC) $(CFLAGS) -c pause.c -Wf-bo1
	$(CC) $(CFLAGS) -c select.c -Wf-bo2
	$(CC) $(CFLAGS) -c highscore.c -Wf-bo2
	$(CC) $(CFLAGS) -c unlocked.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection1.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection2.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection3.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection4.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection_highscore.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection_jukebox.c -Wf-bo2
	$(CC) $(CFLAGS) -c selection_locked.c -Wf-bo2
	$(CC) $(CFLAGS) -c intro.c -Wf-bo3
	$(CC) $(CFLAGS) -c intro_bg.c -Wf-bo3
	$(CC) $(CFLAGS) -c ending.c -Wf-bo3
	$(CC) $(CFLAGS) -c jukebox.c -Wf-bo4
	$(CC) $(CFLAGS) -c data/songs/title_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/mainmenu_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/winscreen_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/highscore_song.asm # bank 4
	$(CC) $(CFLAGS) -c data/songs/plains_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/clouds_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/space_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/dream_song.asm # bank 5
	$(CC) $(CFLAGS) -c data/songs/intro1_song.asm # bank 6
	$(CC) $(CFLAGS) -c winscreen.c -Wf-bo7
	$(CC) $(CFLAGS) -c game_backgrounds.c -Wf-bo7
	$(CC) $(CFLAGS) -c cloud_animations.c -Wf-bo8
	$(CC) $(CFLAGS) -c title.c -Wf-bo8
	$(CC) $(CFLAGS) -c logos.c -Wf-bo9
	$(CC) $(CFLAGS) -c data/songs/ending_song.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/potato_jingle.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/tangram_shine.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/level_clear_song.asm # bank 9
	$(CC) $(CFLAGS) -c sound_data.c -Wf-bo10
	$(CC) $(CFLAGS) -c -Wf-ba0 -c ram.c # ram bank 0
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo16 -Wl-ya1 *.o -o $@

.PHONY: run
run:
	$(EMULATOR) tobu.gb

.PHONY: clean
clean:
	rm *.o
