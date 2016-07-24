songs: $(SONGS_BANK4) $(SONGS_BANK5) $(SONGS_BANK6) $(SONGS_BANK9)

SONGS_BANK4=data/songs/title_song.asm data/songs/mainmenu_song.asm data/songs/score_tally_song.asm data/songs/highscore_song.asm

SONGS_BANK5=data/songs/plains_song.asm data/songs/clouds_song.asm data/songs/space_song.asm

SONGS_BANK6=data/songs/intro_song.asm data/songs/dream_song.asm data/songs/time_out_song.asm

SONGS_BANK9=data/songs/ending_part1_song.asm data/songs/ending_part2_song.asm data/songs/potato_jingle_song.asm data/songs/tangram_shine_song.asm data/songs/tangram_vox_song.asm data/songs/level_clear_song.asm data/songs/unlocked_song.asm

$(SONGS_BANK4): MMLBANK:=4
$(SONGS_BANK5): MMLBANK:=5
$(SONGS_BANK6): MMLBANK:=6
$(SONGS_BANK9): MMLBANK:=9

data/songs/%_song.asm: data/songs/%.mml
	java -jar jar/MMLGB.jar $< $@ $(MMLBANK)
