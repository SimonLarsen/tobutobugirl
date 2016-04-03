CC=lcc
CFLAGS=
IMGTOGB=imgtogb
EMULATOR=gambatte-qt

default: backgrounds sprites tobu.gb

include backgrounds.mk
include sprites.mk
include songs.mk
include sounds.mk

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
	$(CC) $(CFLAGS) -c data/songs/ending2_song.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/potato_jingle.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/tangram_shine.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/level_clear_song.asm # bank 9
	$(CC) $(CFLAGS) -c data/songs/unlocked_song.asm # bank 9
	$(CC) $(CFLAGS) -c sound_data.c -Wf-bo10
	$(CC) $(CFLAGS) -c -Wf-ba0 -c ram.c # ram bank 0
	$(CC) $(CFLAGS) -Wl-yt3 -Wl-yo16 -Wl-ya1 *.o -o $@

.PHONY: run
run:
	$(EMULATOR) tobu.gb

.PHONY: clean
clean:
	rm *.o
