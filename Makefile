CC=lcc
CFLAGS=-Wl-j
IMGTOGB=imgtogb.py
MMLGB=MMLGB.jar

define compile-source
	$(CC) $(CFLAGS) -c $<
endef

default: tobu.gb

include backgrounds.mk
include sprites.mk
include songs.mk
include sounds.mk

OBJ=main.o fade.o gamestate.o cos.o circles.o zoom_circles.o characters.o sound.o \
	mmlgb/driver/music.o mmlgb/driver/freq.o mmlgb/driver/noisefreq.o mmlgb/driver/vib.o arrow.o

OBJ_BANK1=game.o pause.o wipe.o minigamescore.o
OBJ_BANK2=select.o highscore.o unlocked.o \
		  selection1.o selection2.o selection3.o selection4.o \
		  selection_highscore.o selection_jukebox.o selection_locked.o
OBJ_BANK3=intro.o intro_bg.o ending.o
OBJ_BANK4=jukebox.o
OBJ_BANK5=
OBJ_BANK6=
OBJ_BANK7=winscreen.o background1.o background2.o background3.o background4.o 
OBJ_BANK8=title.o pause_cloud1.o pause_cloud2.o
OBJ_BANK9=logos.o win1.o win2.o win3.o win4.o
OBJ_BANK10=sound_data.o

OBJ_ASM=title_song.o mainmenu_song.o score_tally_song.o highscore_song.o plains_song.o \
		clouds_song.o space_song.o dream_song.o dream_score_song.o intro_song.o \
		ending_part1_song.o ending_part2_song.o potato_jingle_song.o tangram_shine_song.o \
		tangram_vox_song.o level_clear_song.o unlocked_song.o time_out_song.o minigame_song.o potaka_song.o

RAM_BANK1=ram.o

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

$(RAM_BANK1): CFLAGS+=-Wf-ba0

arrow.o: arrow.c arrow.h
	${compile-source}

background1.o: background1.c background1.h
	${compile-source}

background2.o: background2.c background2.h
	${compile-source}

background3.o: background3.c background3.h
	${compile-source}

background4.o: background4.c background4.h
	${compile-source}

catface.o: catface.c catface.h
	${compile-source}

characters.o: characters.c characters.h
	${compile-source}

circles.o: circles.c circles.h
	${compile-source}

cos.o: cos.c cos.h
	${compile-source}

ending.o: ending.c defines.h ending.h gamestate.h fade.h intro_bg.h data/bg/ending_thanks.h data/sprite/ending_sprites1.h data/sprite/ending_sprites2.h mmlgb/driver/music.h
	${compile-source}

fade.o: fade.c sound.h fade.h
	${compile-source}

game.o: game.c defines.h game.h fade.h gamestate.h cos.h ram.h highscore.h sound.h mmlgb/driver/music.h pause.h data/bg/hud.h data/bg/clock.h data/sprite/sprites.h data/sprite/portal.h data/sprite/skin1.h data/sprite/skin2.h
	${compile-source}

gamestate.o: gamestate.c defines.h gamestate.h background1.h background2.h background3.h background4.h data/bg/pause_bg.h win1.h win2.h win3.h win4.h pause_cloud1.h pause_cloud2.h mmlgb/driver/music.h
	${compile-source}

highscore.o: highscore.c defines.h gamestate.h fade.h cos.h highscore.h ram.h sound.h characters.h arrow.h data/sprite/empty.h data/bg/highscore.h circles.h selection1.h selection2.h selection3.h selection4.h selection_locked.h
	${compile-source}

intro_bg.o: intro_bg.c intro_bg.h
	${compile-source}

intro.o: intro.c defines.h fade.h gamestate.h intro.h intro_bg.h data/sprite/intro_sprites.h data/sprite/intro_flash.h
	${compile-source}

jukebox.o: jukebox.c defines.h jukebox.h fade.h gamestate.h cos.h sound.h mmlgb/driver/music.h data/bg/jukebox.h data/sprite/digital.h arrow.h data/sprite/notes.h data/sprite/bobblehead.h
	${compile-source}

logos.o: logos.c defines.h gamestate.h logos.h fade.h sound.h mmlgb/driver/music.h data/bg/tangram.h data/bg/potato.h data/sprite/shine.h
	${compile-source}

main.o: main.c gamestate.h main.h ram.h sound.h mmlgb/driver/music.h logos.h intro.h title.h select.h game.h winscreen.h highscore.h unlocked.h jukebox.h ending.h wipe.h minigamescore.h
	${compile-source}

minigamescore.o: minigamescore.c defines.h fade.h gamestate.h sound.h ram.h data/bg/minigame_score_bg.h characters.h zoom_circles.h
	${compile-source}

pause.o: pause.c defines.h gamestate.h fade.h sound.h ram.h mmlgb/driver/music.h characters.h
	${compile-source}

ram.o: ram.c 
	${compile-source}

select.o: select.c defines.h select.h fade.h gamestate.h cos.h ram.h sound.h mmlgb/driver/music.h characters.h arrow.h data/sprite/togglecat.h circles.h data/bg/catface.h data/bg/select.h selection1.h selection2.h selection3.h selection4.h selection_highscore.h selection_jukebox.h selection_locked.h
	${compile-source}

selection1.o: selection1.c selection1.h
	${compile-source}

selection2.o: selection2.c selection2.h
	${compile-source}

selection3.o: selection3.c selection3.h
	${compile-source}

selection4.o: selection4.c selection4.h
	${compile-source}

selection_highscore.o: selection_highscore.c selection_highscore.h
	${compile-source}

selection_jukebox.o: selection_jukebox.c selection_jukebox.h
	${compile-source}

selection_locked.o: selection_locked.c selection_locked.h
	${compile-source}

sound.o: sound.c sound.h gamestate.h mmlgb/driver/music.h mmlgb/driver/notes.h mmlgb/driver/freq.h mmlgb/driver/noisefreq.h mmlgb/driver/vib.h
	${compile-source}

sound_data.o: sound_data.c data/sounds/sfx_bump.h data/sounds/sfx_bump_alien.h data/sounds/sfx_cat_disable.h data/sounds/sfx_cat_enable.h data/sounds/sfx_dash.h data/sounds/sfx_highscore_switch.h data/sounds/sfx_jetpack.h data/sounds/sfx_menu_cancel.h data/sounds/sfx_menu_confirm.h data/sounds/sfx_menu_locked.h data/sounds/sfx_menu_switch.h data/sounds/sfx_player_die.h data/sounds/sfx_stomp_alien.h data/sounds/sfx_stomp_bat.h data/sounds/sfx_stomp_bird.h data/sounds/sfx_stomp_ghost.h data/sounds/sfx_time_low.h data/sounds/sfx_time_out.h data/sounds/sfx_time_pickup.h data/sounds/sfx_warp_start.h data/sounds/sfx_rank_crash.h
	${compile-source}

title.o: title.c defines.h title.h fade.h gamestate.h sound.h ram.h data/bg/titlescreen.h data/bg/titlescreen_bg.h characters.h data/sprite/title_cat.h
	${compile-source}

unlocked.o: unlocked.c defines.h unlocked.h fade.h gamestate.h characters.h zoom_circles.h data/bg/unlocked.h selection2.h selection3.h selection4.h selection_jukebox.h
	${compile-source}

win1.o: win1.c win1.h
	${compile-source}

win2.o: win2.c win2.h
	${compile-source}

win3.o: win3.c win3.h
	${compile-source}

win4.o: win4.c win4.h
	${compile-source}

winscreen.o: winscreen.c defines.h gamestate.h fade.h winscreen.h sound.h characters.h data/bg/win_base.h data/bg/rank_banner.h data/sprite/ranks.h circles.h mmlgb/driver/music.h mmlgb/driver/notes.h mmlgb/driver/freq.h
	${compile-source}

wipe.o: wipe.c main.h wipe.h defines.h fade.h gamestate.h sound.h circles.h data/bg/wipe.h data/sprite/wipe_marker.h
	${compile-source}

zoom_circles.o: zoom_circles.c zoom_circles.h
	${compile-source}

%.o: data/songs/%.asm
	$(CC) $(CFLAGS) -c $< -o $@

tobu.gb: ram.o $(OBJ) $(OBJ_BANK1) $(OBJ_BANK2) $(OBJ_BANK3) $(OBJ_BANK4) $(OBJ_BANK5) \
	$(OBJ_BANK6) $(OBJ_BANK7) $(OBJ_BANK8) $(OBJ_BANK9) $(OBJ_BANK10) $(OBJ_ASM)
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo16 -Wl-ya1 $^ -o $@
