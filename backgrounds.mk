backgrounds: logos_backgrounds intro_backgrounds title_backgrounds select_backgrounds ingame_backgrounds pause_backgrounds winscreen_backgrounds highscore_backgrounds ending_backgrounds unlocked_backgrounds jukebox_backgrounds wipe_backgrounds

logos_backgrounds: data/bg/tangram.h

data/bg/tangram.h: data/bg/tangram.png
	$(IMGTOGB) --map --rle $< $@

data/bg/potato.h: data/bg/potato.png
	$(IMGTOGB) --map --rle $< $@

intro_backgrounds:

title_backgrounds: data/bg/titlescreen.h

data/bg/titlescreen.h: data/bg/titlescreen.png
	$(IMGTOGB) --map $< $@

select_backgrounds: data/bg/catface.h data/bg/select.h

data/bg/catface.h: data/bg/catface.png
	$(IMGTOGB) --map -O 9 $< $@

data/bg/select.h: data/bg/select.png
	$(IMGTOGB) --map --rle -O 13 $< $@

ingame_backgrounds: data/bg/hud.h data/bg/clock.h data/bg/background1.h data/bg/background2.h data/bg/background3.h data/bg/background4.h

data/bg/hud.h: data/bg/hud.png
	$(IMGTOGB) --map $< $@

data/bg/clock.h: data/bg/clock.png
	$(IMGTOGB) --map -O 20 $< $@

data/bg/background1.h: data/bg/background1.png
	$(IMGTOGB) --map -O 34 --rle $< $@

data/bg/background2.h: data/bg/background2.png
	$(IMGTOGB) --map -O 34 --rle $< $@

data/bg/background3.h: data/bg/background3.png
	$(IMGTOGB) --map -O 34 --rle $< $@

data/bg/background4.h: data/bg/background4.png
	$(IMGTOGB) --map -O 34 --rle $< $@

pause_backgrounds: data/bg/pause_bg.h

data/bg/pause_bg.h: data/bg/pause_bg.png
	$(IMGTOGB) --map --rle -O 38 $< $@

winscreen_backgrounds: data/bg/win_base.h data/bg/win1.h data/bg/win2.h data/bg/win3.h data/bg/win4.h

data/bg/win_base.h: data/bg/win_base.png
	$(IMGTOGB) --map -O 40 --rle $< $@

data/bg/win1.h: data/bg/win1.png
	$(IMGTOGB) --map -O 69 --rle $< $@

data/bg/win2.h: data/bg/win2.png
	$(IMGTOGB) --map -O 69 --rle $< $@

data/bg/win3.h: data/bg/win3.png
	$(IMGTOGB) --map -O 69 --rle $< $@

data/bg/win4.h: data/bg/win4.png
	$(IMGTOGB) --map -O 69 --rle $< $@

highscore_backgrounds: data/bg/highscore.h data/sprite/empty.h

data/bg/highscore.h: data/bg/highscore.png
	$(IMGTOGB) --map -O 47 $< $@

data/sprite/empty.h: data/sprite/empty.png
	$Ma(IMGTOGB) --sprite $< $@

ending_backgrounds: data/bg/ending_thanks.h

data/bg/ending_thanks.h: data/bg/ending_thanks.png
	$(IMGTOGB) --map -O 174 --rle $< $@

unlocked_backgrounds: data/bg/unlocked.h data/bg/zoom_circles.h

data/bg/unlocked.h: data/bg/unlocked.png
	$(IMGTOGB) --map -O 47 $< $@

data/bg/zoom_circles.h: data/bg/zoom_circles.png
	$(IMGTOGB) --map $< $@

jukebox_backgrounds: data/bg/jukebox.h

data/bg/jukebox.h: data/bg/jukebox.png
	$(IMGTOGB) --map --rle -O 37 $< $@

wipe_backgrounds: data/bg/wipe.h

data/bg/wipe.h: data/bg/wipe.png
	$(IMGTOGB) --map --rle  -O 13 $< $@
