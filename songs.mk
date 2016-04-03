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
	java -jar jar/MMLGB.jar data/mml/ending_cutscene_part_2.mml data/songs/ending2_song.asm 9
	java -jar jar/MMLGB.jar data/mml/potato_jingle.mml data/songs/potato_jingle.asm 9
	java -jar jar/MMLGB.jar data/mml/tangram_shine.mml data/songs/tangram_shine.asm 9
	java -jar jar/MMLGB.jar data/mml/level_clear.mml data/songs/level_clear_song.asm 9
	java -jar jar/MMLGB.jar data/mml/unlocked.mml data/songs/unlocked_song.asm 9
