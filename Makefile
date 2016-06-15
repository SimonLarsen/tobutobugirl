CC=lcc
CFLAGS=-Wl-j
IMGTOGB=imgtogb
EMULATOR=gambatte-qt

default: backgrounds sprites sounds songs tobu.gb

include backgrounds.mk
include sprites.mk
include songs.mk
include sounds.mk

OBJ=main.o fade.o gamestate.o cos.o circles.o characters.o sound.o mmlgb/driver/music.o mmlgb/driver/freq.o mmlgb/driver/noisefreq.o mmlgb/driver/vib.o arrow.o

OBJ_BANK1=game.o pause.o

OBJ_BANK2=select.o highscore.o unlocked.o selection1.o selection2.o selection3.o selection4.o selection_highscore.o selection_jukebox.o selection_locked.o

OBJ_BANK3=intro.o intro_bg.o ending.o

OBJ_BANK4=jukebox.o

OBJ_BANK5=
OBJ_BANK6=
OBJ_BANK7=winscreen.o game_backgrounds.o

OBJ_BANK8=cloud_animations.o title.o

OBJ_BANK9=logos.o

OBJ_BANK10=sound_data.o

OBJ_ASM=title_song.o mainmenu_song.o score_tally_song.o highscore_song.o plains_song.o clouds_song.o space_song.o dream_song.o intro_song.o ending_part1_song.o ending_part2_song.o potato_jingle_song.o tangram_shine_song.o tangram_vox_song.o level_clear_song.o unlocked_song.o

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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: data/songs/%.asm
	$(CC) $(CFLAGS) -c $< -o $@

ram.o: ram.c
	$(CC) $(CFLAGS) -c -Wf-ba0 -c $< -o $@ # ram bank 0

tobu.gb: $(OBJ) $(OBJ_BANK1) $(OBJ_BANK2) $(OBJ_BANK3) $(OBJ_BANK4) $(OBJ_BANK5) $(OBJ_BANK6) $(OBJ_BANK7) $(OBJ_BANK8) $(OBJ_BANK9) $(OBJ_BANK10) $(OBJ_ASM) ram.o
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo16 -Wl-ya1 $^ -o $@

.PHONY: run
run:
	$(EMULATOR) tobu.gb

.PHONY: clean
clean:
	rm *.o
	rm data/songs/*.asm
	rm data/sounds/*.h
