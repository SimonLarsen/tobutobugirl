songs: $(SONGS_BANK4) $(SONGS_BANK5) $(SONGS_BANK6) $(SONGS_BANK9)

SONGS_BANK4=data/mml/title_song.asm data/mml/mainmenu_song.asm data/mml/score_tally_song.asm data/mml/highscore_song.asm

SONGS_BANK5=data/mml/plains_song.asm data/mml/clouds_song.asm data/mml/space_song.asm data/mml/dream_song.asm

SONGS_BANK6=data/mml/intro_song.asm

SONGS_BANK9=data/mml/ending_part1_song.asm data/mml/ending_part2_song.asm data/mml/potato_jingle_song.asm data/mml/tangram_shine_song.asm data/mml/level_clear_song.asm data/mml/unlocked_song.asm

$(SONGS_BANK4): MMLBANK:=4
$(SONGS_BANK5): MMLBANK:=5
$(SONGS_BANK6): MMLBANK:=6
$(SONGS_BANK9): MMLBANK:=9

data/mml/%_song.asm: data/mml/%.mml
	java -jar jar/MMLGB.jar $< $@ $(MMLBANK)
