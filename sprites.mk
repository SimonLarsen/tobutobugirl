define make-sprite-8x16
	$(IMGTOGB) --s8x16 $< $@
endef

define make-sprite-8x8
	$(IMGTOGB) $< $@
endef

sprites: data/sprite/sprites.h data/sprite/portal.h data/sprite/skin1.h data/sprite/skin2.h data/sprite/notes.h data/sprite/intro_sprites.h data/sprite/intro_flash.h data/sprite/bobblehead.h data/sprite/ending_sprites1.h data/sprite/ending_sprites2.h data/sprite/digital.h data/sprite/select_arrow.h data/sprite/togglecat.h data/sprite/pause_cloud1.h data/sprite/pause_cloud2.h data/sprite/shine.h data/sprite/title_cat.h data/sprite/wipe_marker.h data/sprite/ranks.h

data/sprite/sprites.h: data/sprite/sprites.png
	$(make-sprite-8x16)

data/sprite/portal.h: data/sprite/portal.png
	$(make-sprite-8x16)

data/sprite/skin1.h: data/sprite/skin1.png
	$(make-sprite-8x16)

data/sprite/skin2.h: data/sprite/skin2.png
	$(make-sprite-8x16)

data/sprite/notes.h: data/sprite/notes.png
	$(make-sprite-8x16)

data/sprite/intro_sprites.h: data/sprite/intro_sprites.png
	$(make-sprite-8x16)

data/sprite/intro_flash.h: data/sprite/intro_flash.png
	$(make-sprite-8x16)

data/sprite/bobblehead.h: data/sprite/bobblehead.png
	$(make-sprite-8x16)

data/sprite/ending_sprites1.h: data/sprite/ending_sprites1.png
	$(make-sprite-8x16)

data/sprite/ending_sprites2.h: data/sprite/ending_sprites2.png
	$(make-sprite-8x16)

data/sprite/digital.h: data/sprite/digital.png
	$(make-sprite-8x8)

data/sprite/select_arrow.h: data/sprite/select_arrow.png
	$(make-sprite-8x16)

data/sprite/togglecat.h: data/sprite/togglecat.png
	$(make-sprite-8x8)

data/sprite/pause_cloud1.h: data/sprite/pause_cloud1.png
	$(make-sprite-8x16)

data/sprite/pause_cloud2.h: data/sprite/pause_cloud2.png
	$(make-sprite-8x16)

data/sprite/shine.h: data/sprite/shine.png
	$(make-sprite-8x16)

data/sprite/title_cat.h: data/sprite/title_cat.png
	$(make-sprite-8x16)

data/sprite/wipe_marker.h: data/sprite/wipe_marker.png
	$(make-sprite-8x16)

data/sprite/ranks.h: data/sprite/ranks.png
	$(make-sprite-8x16)

#$(IMGTOGB) --map data/bg/circles.png data/bg/circles.h
#$(IMGTOGB) --sprite data/sprite/characters.png data/sprite/characters.h # 40 tiles
#$(IMGTOGB) --sprite --8x16 data/sprite/arrow.png data/sprite/arrow.h
#$(IMGTOGB) --map -O 90 data/bg/selection1.png data/bg/selection1.h
#$(IMGTOGB) --map -O 90 data/bg/selection2.png data/bg/selection2.h
#$(IMGTOGB) --map -O 90 data/bg/selection3.png data/bg/selection3.h
#$(IMGTOGB) --map -O 90 data/bg/selection4.png data/bg/selection4.h
#$(IMGTOGB) --map -O 90 data/bg/selection_highscore.png data/bg/selection_highscore.h
#$(IMGTOGB) --map -O 90 data/bg/selection_jukebox.png data/bg/selection_jukebox.h
#$(IMGTOGB) --map -O 90 data/bg/selection_locked.png data/bg/selection_locked.h
#$(IMGTOGB) --map --rle data/bg/intro_bg.png data/bg/intro_bg.h
