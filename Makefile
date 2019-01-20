CC=lcc
CFLAGS=-Wl-j
IMGTOGB=imgtogb
EMULATOR=gambatte_sdl -s 3
MMLGB=MMLGB.jar

default: tobu.gb

include backgrounds.mk
include sprites.mk
include songs.mk
include sounds.mk

OBJ=main.o fade.o gamestate.o cos.o circles.o zoom_circles.o characters.o sound.o mmlgb/driver/music.o mmlgb/driver/freq.o mmlgb/driver/noisefreq.o mmlgb/driver/vib.o arrow.o

OBJ_BANK1=game.o pause.o wipe.o minigamescore.o
OBJ_BANK2=select.o highscore.o unlocked.o selection1.o selection2.o selection3.o selection4.o selection_highscore.o selection_jukebox.o selection_locked.o
OBJ_BANK3=intro.o intro_bg.o ending.o
OBJ_BANK4=jukebox.o
OBJ_BANK5=
OBJ_BANK6=
OBJ_BANK7=winscreen.o game_backgrounds.o
OBJ_BANK8=cloud_animations.o title.o
OBJ_BANK9=logos.o win_backgrounds.o
OBJ_BANK10=sound_data.o

OBJ_ASM=title_song.o mainmenu_song.o score_tally_song.o highscore_song.o plains_song.o clouds_song.o space_song.o dream_song.o dream_score_song.o intro_song.o ending_part1_song.o ending_part2_song.o potato_jingle_song.o tangram_shine_song.o tangram_vox_song.o level_clear_song.o unlocked_song.o time_out_song.o minigame_song.o potaka_song.o

$(OBJ_BANK1): CFLAGS+=-Wf-bo1
$(OBJ_BANK2): CFLAGS+=-Wf-bo2
$(OBJ_BANK3): CFLAGS+=-Wf-bo3
$(OBJ_BANK4): CFLAGS+=-Wf-bo4
$(OBJ_BANK5): CFLAGS+=-Wf-bo5
$(OBJ_BANK6): CFLAGS+=-Wf-bo6
$(OBJ_BANK7): CFLAGS+=-Wf-bo7
$(OBJ_BANK8): CFLAGS+=-Wf-bo8
$(OBJ_BANK9): CFLAGS+=-Wf-bo9
$(OBJ_BANK10): CFLAGS+=-Wf-bo10
$(OBJ_BANK11): CFLAGS+=-Wf-bo11

arrow.o: arrow.c data/sprite/arrow.h
	$(CC) $(CFLAGS) -c $<

characters.o: characters.c data/sprite/characters.h
	$(CC) $(CFLAGS) -c $<

circles.o: circles.c data/bg/circles.h
	$(CC) $(CFLAGS) -c $<

cloud_animations.o: cloud_animations.c cloud_animations.h data/sprite/pause_cloud1.h data/sprite/pause_cloud2.h
	$(CC) $(CFLAGS) -c $<

cos.o: cos.c cos.h
	$(CC) $(CFLAGS) -c $<

ending.o: ending.c defines.h ending.h gamestate.h fade.h data/bg/intro_bg.h data/bg/ending_thanks.h data/sprite/ending_sprites1.h data/sprite/ending_sprites2.h mmlgb/driver/music.h
	$(CC) $(CFLAGS) -c $<

fade.o: fade.c sound.h fade.h
	$(CC) $(CFLAGS) -c $<

game_backgrounds.o: game_backgrounds.c game_backgrounds.h data/bg/background1.h data/bg/background2.h data/bg/background3.h data/bg/background4.h data/bg/pause_bg.h
	$(CC) $(CFLAGS) -c $<

game.o: game.c defines.h game.h fade.h gamestate.h cos.h ram.h highscore.h sound.h mmlgb/driver/music.h pause.h data/bg/hud.h data/bg/clock.h data/sprite/sprites.h data/sprite/portal.h data/sprite/skin1.h data/sprite/skin2.h
	$(CC) $(CFLAGS) -c $<

gamestate.o: gamestate.c defines.h gamestate.h game_backgrounds.h win_backgrounds.h cloud_animations.h mmlgb/driver/music.h
	$(CC) $(CFLAGS) -c $<

highscore.o: highscore.c defines.h gamestate.h fade.h cos.h highscore.h ram.h sound.h data/sprite/characters.h data/sprite/arrow.h data/sprite/empty.h data/bg/circles.h data/bg/highscore.h data/bg/selection1.h data/bg/selection2.h data/bg/selection3.h data/bg/selection4.h data/bg/selection_locked.h
	$(CC) $(CFLAGS) -c $<

intro_bg.o: intro_bg.c data/bg/intro_bg.h
	$(CC) $(CFLAGS) -c $<

intro.o: intro.c defines.h fade.h gamestate.h intro.h data/bg/intro_bg.h data/sprite/intro_sprites.h data/sprite/intro_flash.h
	$(CC) $(CFLAGS) -c $<

jukebox.o: jukebox.c defines.h jukebox.h fade.h gamestate.h cos.h sound.h mmlgb/driver/music.h data/bg/jukebox.h data/sprite/digital.h data/sprite/arrow.h data/sprite/notes.h data/sprite/bobblehead.h
	$(CC) $(CFLAGS) -c $<

logos.o: logos.c defines.h gamestate.h logos.h fade.h sound.h mmlgb/driver/music.h data/bg/tangram.h data/bg/potato.h data/sprite/shine.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c gamestate.h main.h ram.h sound.h mmlgb/driver/music.h logos.h intro.h title.h select.h game.h winscreen.h highscore.h unlocked.h jukebox.h ending.h wipe.h minigamescore.h
	$(CC) $(CFLAGS) -c $<

minigamescore.o: minigamescore.c defines.h fade.h gamestate.h sound.h ram.h data/sprite/characters.h data/bg/minigame_score_bg.h data/bg/zoom_circles.h
	$(CC) $(CFLAGS) -c $<

pause.o: pause.c defines.h gamestate.h fade.h sound.h ram.h mmlgb/driver/music.h data/sprite/characters.h
	$(CC) $(CFLAGS) -c $<

select.o: select.c defines.h select.h fade.h gamestate.h cos.h ram.h sound.h mmlgb/driver/music.h data/sprite/characters.h data/sprite/arrow.h data/sprite/togglecat.h data/bg/circles.h data/bg/catface.h data/bg/select.h data/bg/selection1.h data/bg/selection2.h data/bg/selection3.h data/bg/selection4.h data/bg/selection_highscore.h data/bg/selection_jukebox.h data/bg/selection_locked.h
	$(CC) $(CFLAGS) -c $<

selection1.o: selection1.c data/bg/selection1.h
	$(CC) $(CFLAGS) -c $<

selection2.o: selection2.c data/bg/selection2.h
	$(CC) $(CFLAGS) -c $<

selection3.o: selection3.c data/bg/selection3.h
	$(CC) $(CFLAGS) -c $<

selection4.o: selection4.c data/bg/selection4.h
	$(CC) $(CFLAGS) -c $<

selection_highscore.o: selection_highscore.c data/bg/selection_highscore.h
	$(CC) $(CFLAGS) -c $<

selection_jukebox.o: selection_jukebox.c data/bg/selection_jukebox.h
	$(CC) $(CFLAGS) -c $<

selection_locked.o: selection_locked.c data/bg/selection_locked.h
	$(CC) $(CFLAGS) -c $<

sound.o: sound.c sound.h gamestate.h mmlgb/driver/music.h mmlgb/driver/notes.h mmlgb/driver/freq.h mmlgb/driver/noisefreq.h mmlgb/driver/vib.h
	$(CC) $(CFLAGS) -c $<

sound_data.o: sound_data.c data/sounds/sfx_bump.h data/sounds/sfx_bump_alien.h data/sounds/sfx_cat_disable.h data/sounds/sfx_cat_enable.h data/sounds/sfx_dash.h data/sounds/sfx_highscore_switch.h data/sounds/sfx_jetpack.h data/sounds/sfx_menu_cancel.h data/sounds/sfx_menu_confirm.h data/sounds/sfx_menu_locked.h data/sounds/sfx_menu_switch.h data/sounds/sfx_player_die.h data/sounds/sfx_stomp_alien.h data/sounds/sfx_stomp_bat.h data/sounds/sfx_stomp_bird.h data/sounds/sfx_stomp_ghost.h data/sounds/sfx_time_low.h data/sounds/sfx_time_out.h data/sounds/sfx_time_pickup.h data/sounds/sfx_warp_start.h data/sounds/sfx_rank_crash.h
	$(CC) $(CFLAGS) -c $<

title.o: title.c defines.h title.h fade.h gamestate.h sound.h ram.h data/bg/titlescreen.h data/bg/titlescreen_bg.h data/sprite/characters.h data/sprite/title_cat.h
	$(CC) $(CFLAGS) -c $<

unlocked.o: unlocked.c defines.h unlocked.h fade.h gamestate.h data/sprite/characters.h data/bg/zoom_circles.h data/bg/unlocked.h data/bg/selection2.h data/bg/selection3.h data/bg/selection4.h data/bg/selection_jukebox.h
	$(CC) $(CFLAGS) -c $<

win_backgrounds.o: win_backgrounds.c win_backgrounds.h data/bg/win1.h data/bg/win2.h data/bg/win3.h data/bg/win4.h
	$(CC) $(CFLAGS) -c $<

winscreen.o: winscreen.c defines.h gamestate.h fade.h winscreen.h sound.h data/bg/circles.h data/sprite/characters.h data/bg/win_base.h data/bg/rank_banner.h data/sprite/ranks.h mmlgb/driver/music.h mmlgb/driver/notes.h mmlgb/driver/freq.h
	$(CC) $(CFLAGS) -c $<

wipe.o: wipe.c main.h wipe.h defines.h fade.h gamestate.h sound.h data/bg/circles.h data/bg/wipe.h data/sprite/wipe_marker.h
	$(CC) $(CFLAGS) -c $<

zoom_circles.o: zoom_circles.c data/bg/zoom_circles.h
	$(CC) $(CFLAGS) -c $<

ram.o: ram.c
	$(CC) $(CFLAGS) -c -Wf-ba0 -c $< -o $@ # ram bank 0

%.o: data/songs/%.asm
	$(CC) $(CFLAGS) -c $< -o $@

tobu.gb: $(OBJ) $(OBJ_BANK1) $(OBJ_BANK2) $(OBJ_BANK3) $(OBJ_BANK4) $(OBJ_BANK5) $(OBJ_BANK6) $(OBJ_BANK7) $(OBJ_BANK8) $(OBJ_BANK9) $(OBJ_BANK10) $(OBJ_ASM) ram.o
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo16 -Wl-ya1 $^ -o $@
