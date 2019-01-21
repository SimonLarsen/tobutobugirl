backgrounds: logos_backgrounds intro_backgrounds title_backgrounds select_backgrounds ingame_backgrounds pause_backgrounds winscreen_backgrounds highscore_backgrounds ending_backgrounds unlocked_backgrounds jukebox_backgrounds wipe_backgrounds minigame_score_backgrounds selection_backgrounds common_backgrounds

logos_backgrounds: data/bg/tangram.h data/bg/potato.h

data/bg/tangram.h: data/bg/tangram.png
	$(IMGTOGB) --map --rle $< $@

data/bg/potato.h: data/bg/potato.png
	$(IMGTOGB) --map --rle $< $@

intro_backgrounds:

title_backgrounds: data/bg/titlescreen.h data/bg/titlescreen_bg.h

data/bg/titlescreen.h: data/bg/titlescreen.png
	$(IMGTOGB) --map $< $@

data/bg/titlescreen_bg.h: data/bg/titlescreen_bg.png
	$(IMGTOGB) --map -O 90 $< $@

select_backgrounds: data/bg/catface.h data/bg/select.h

data/bg/catface.h: data/bg/catface.png
	$(IMGTOGB) --map -O 9 $< $@

data/bg/select.h: data/bg/select.png
	$(IMGTOGB) --map --rle -O 13 $< $@

ingame_backgrounds: data/bg/hud.h data/bg/clock.h background1.h background2.h background3.h background4.h

data/bg/hud.h: data/bg/hud.png
	$(IMGTOGB) --map $< $@

data/bg/clock.h: data/bg/clock.png
	$(IMGTOGB) --map -O 20 $< $@

background1.h background1.c: data/bg/background1.png
	$(IMGTOGB) --map -O 34 --rle $< -C background1.c background1.h

background2.h background2.c: data/bg/background2.png
	$(IMGTOGB) --map -O 34 --rle $< -C background2.c background2.h

background3.h background3.c: data/bg/background3.png
	$(IMGTOGB) --map -O 34 --rle $< -C background3.c background3.h

background4.h background4.c: data/bg/background4.png
	$(IMGTOGB) --map -O 34 --rle $< -C background4.c background4.h

pause_backgrounds: data/bg/pause_bg.h

data/bg/pause_bg.h: data/bg/pause_bg.png
	$(IMGTOGB) --map --rle -O 38 $< $@

winscreen_backgrounds: data/bg/win_base.h data/bg/rank_banner.h win1.h win2.h win3.h win4.h

data/bg/win_base.h: data/bg/win_base.png
	$(IMGTOGB) --map -O 40 --rle $< $@

data/bg/rank_banner.h: data/bg/rank_banner.png
	$(IMGTOGB) --map -O 69 --rle $< $@

win1.c win1.h: data/bg/win1.png
	$(IMGTOGB) --map -O 84 --rle $< -C win1.c win1.h

win2.c win2.h: data/bg/win2.png
	$(IMGTOGB) --map -O 84 --rle $< -C win2.c win2.h

win3.c win3.h: data/bg/win3.png
	$(IMGTOGB) --map -O 84 --rle $< -C win3.c win3.h

win4.c win4.h: data/bg/win4.png
	$(IMGTOGB) --map -O 84 --rle $< -C win4.c win4.h

highscore_backgrounds: data/bg/highscore.h data/sprite/empty.h

data/bg/highscore.h: data/bg/highscore.png
	$(IMGTOGB) --map -O 47 $< $@

data/sprite/empty.h: data/sprite/empty.png
	$(IMGTOGB) $< $@

ending_backgrounds: data/bg/ending_thanks.h

data/bg/ending_thanks.h: data/bg/ending_thanks.png
	$(IMGTOGB) --map -O 174 --rle $< $@

unlocked_backgrounds: data/bg/unlocked.h

data/bg/unlocked.h: data/bg/unlocked.png
	$(IMGTOGB) --map -O 47 $< $@

#data/bg/zoom_circles.h: data/bg/zoom_circles.png
#	$(IMGTOGB) --map $< $@

jukebox_backgrounds: data/bg/jukebox.h

data/bg/jukebox.h: data/bg/jukebox.png
	$(IMGTOGB) --map --rle -O 37 $< $@

wipe_backgrounds: data/bg/wipe.h

data/bg/wipe.h: data/bg/wipe.png
	$(IMGTOGB) --map --rle -O 13 $< $@

minigame_score_backgrounds: data/bg/minigame_score_bg.h

data/bg/minigame_score_bg.h: data/bg/minigame_score_bg.png
	$(IMGTOGB) --map --rle -O 18 $< $@

selection_backgrounds: selection1.c selection1.h selection2.c selection2.h selection3.c selection3.h selection4.c selection4.h selection_highscore.c selection_highscore.h selection_jukebox.c selection_jukebox.h selection_locked.c selection_locked.h data/bg/catface.h

selection1.c selection1.h: data/bg/selection1.png
	$(IMGTOGB) --map -O 90 $< -C selection1.c selection1.h

selection2.c selection2.h: data/bg/selection2.png
	$(IMGTOGB) --map -O 90 $< -C selection2.c selection2.h

selection3.c selection3.h: data/bg/selection3.png
	$(IMGTOGB) --map -O 90 $< -C selection3.c selection3.h

selection4.c selection4.h: data/bg/selection4.png
	$(IMGTOGB) --map -O 90 $< -C selection4.c selection4.h

selection_highscore.c selection_highscore.h: data/bg/selection_highscore.png
	$(IMGTOGB) --map -O 90 $< -C selection_highscore.c selection_highscore.h

selection_jukebox.c selection_jukebox.h: data/bg/selection_jukebox.png
	$(IMGTOGB) --map -O 90 $< -C selection_jukebox.c selection_jukebox.h

selection_locked.c selection_locked.h: data/bg/selection_locked.png
	$(IMGTOGB) --map -O 90 $< -C selection_locked.c selection_locked.h

common_backgrounds: circles.c circles.h intro_bg.c intro_bg.h zoom_circles.c zoom_circles.h characters.c characters.h

circles.c circles.h: data/bg/circles.png
	$(IMGTOGB) --map $< -C circles.c circles.h

intro_bg.c intro_bg.h: data/bg/intro_bg.png
	$(IMGTOGB) --map $< --rle -C intro_bg.c intro_bg.h

zoom_circles.c zoom_circles.h: data/bg/zoom_circles.png
	$(IMGTOGB) --map $< -C zoom_circles.c zoom_circles.h

characters.c characters.h: data/sprite/characters.png
	$(IMGTOGB) $< -C characters.c characters.h
