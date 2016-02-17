#include <gb/gb.h>
#include "defines.h"
#include "select.h"
#include "fade.h"
#include "gamestate.h"
#include "cos.h"
#include "ram.h"
#include "sound.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"
#include "data/sprite/togglecat.h"

#include "data/bg/circles.h"
#include "data/bg/catface.h"
#include "data/bg/select.h"

#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"
#include "data/bg/selection4.h"
#include "data/bg/selection_highscore.h"
#include "data/bg/selection_jukebox.h"
#include "data/bg/selection_locked.h"

UBYTE select_circle_index;
UBYTE select_arrow_offset1;
UBYTE select_arrow_offset2;
UBYTE select_scroll_dir;
UBYTE select_cat_state;
UBYTE select_cat_frame;
UBYTE select_cat_frame_reverse;

#define CAT_OFF 0U
#define CAT_IN  1U
#define CAT_ON  2U
#define CAT_OUT 3U

extern UBYTE mainmenu_song_data;

void initSelect() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_sprite_data(0U, 37U, characters_data);
	set_sprite_data(37U, arrow_data_length, arrow_data);
	set_sprite_data(41U, togglecat_data_length, togglecat_data);

	set_bkg_data(0U, circles_data_length, circles_data);
	set_bkg_data(catface_offset, catface_data_length, catface_data);
	set_bkg_data_rle(select_offset, select_data_length, select_data);
	set_bkg_tiles_rle(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

	ticks = 0U;
	select_circle_index = 0U;
	select_arrow_offset1 = 0U;
	select_arrow_offset2 = 0U;

	select_cat_state = CAT_OFF;
	select_cat_frame = 0U;
	select_cat_frame_reverse = 0U;
	if(player_skin == 2U) select_cat_state = CAT_ON;

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xB4U; // 11100100
	BGP_REG = 0xB4U; // 11100100

	clearSprites();
	_selectUpdateScreen();

	setMusicBank(4U);
	playMusic(&mainmenu_song_data);

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x8;

	DISPLAY_ON;
	enable_interrupts();
}

UBYTE *selectGetBannerData() {
	if(selection <= 4U && selection > levels_completed+1U) {
		set_bkg_data(selection_locked_offset, selection_locked_data_length, selection_locked_data);
		return selection_locked_tiles;
	} else if(selection == 1U) {
		set_bkg_data(selection1_offset, selection1_data_length, selection1_data);
		return selection1_tiles;
	} else if(selection == 2U) {
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		return selection2_tiles;
	} else if(selection == 3U) {
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		return selection3_tiles;
	} else if(selection == 4U) {
		set_bkg_data(selection4_offset, selection4_data_length, selection4_data);
		return selection4_tiles;
	} else if(selection == 5U) {
		set_bkg_data(selection_jukebox_offset, selection_jukebox_data_length, selection_jukebox_data);
		return selection_jukebox_tiles;
	} else if(selection == 6U) {
		set_bkg_data(selection_highscore_offset, selection_highscore_data_length, selection_highscore_data);
		return selection_highscore_tiles;
	}
	return 0U;
}

void _selectUpdateScreen() {
	UBYTE *data = selectGetBannerData();
	set_bkg_tiles(0U, 10U, 20U, 6U, data);
}

void selectUpdateSprites() {
	UBYTE frame;

	setSprite(24U-select_arrow_offset1, 69U, 37U, OBJ_PAL0);
	setSprite(32U-select_arrow_offset1, 69U, 39U, OBJ_PAL0);
	setSprite(24U-select_arrow_offset1, 77U, 38U, OBJ_PAL0);
	setSprite(32U-select_arrow_offset1, 77U, 40U, OBJ_PAL0);

	setSprite(136U+select_arrow_offset2, 69U, 39U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+select_arrow_offset2, 69U, 37U, OBJ_PAL0 | FLIP_X);
	setSprite(136U+select_arrow_offset2, 77U, 40U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+select_arrow_offset2, 77U, 38U, OBJ_PAL0 | FLIP_X);

	if(levels_completed >= 3U) {
		switch(select_cat_state) {
			case CAT_OFF:
				if((ticks & 15U) == 15U) {
					if(select_cat_frame_reverse) {
						select_cat_frame--;
					} else {
						select_cat_frame++;
					}
					if(select_cat_frame == 0U) select_cat_frame_reverse = 0U;
					else if(select_cat_frame == 4U) select_cat_frame_reverse = 1U;
				}
				if(CLICKED(J_SELECT)) {
					select_cat_state = CAT_IN;
					player_skin = 2U;
					playSound(SFX_CAT_ENABLE);
				}
				break;
			case CAT_IN:
				if((ticks & 7U) == 7U) select_cat_frame++;
				if(select_cat_frame == 8U) select_cat_state = CAT_ON;
				break;
			case CAT_ON:
				if((ticks & 15U) == 15U) select_cat_frame++;
				if(select_cat_frame == 10U) select_cat_frame = 8U;
				if(CLICKED(J_SELECT)) {
					select_cat_state = CAT_OUT;
					player_skin = 1U;
				}
				break;
			case CAT_OUT:
				if((ticks & 7U) == 7U) select_cat_frame--;
				if(select_cat_frame == 4U) {
					select_cat_state = CAT_OFF;
					select_cat_frame_reverse = 1U;
					playSound(SFX_CAT_DISABLE);
				}
				break;
		}

		frame = 41U + (select_cat_frame << 2);
		setSprite(136U, 20U, frame++, OBJ_PAL0);
		setSprite(144U, 20U, frame++, OBJ_PAL0);
		setSprite(136U, 28U, frame++, OBJ_PAL0);
		setSprite(144U, 28U, frame, OBJ_PAL0);
	}
}

void selectScrollCircles() {
	select_circle_index = (select_circle_index+1U) & 7U;
	set_bkg_data(13U, 1U, &circles_data[(select_circle_index << 4)]);
}

void selectFadeOut() {
	UBYTE i, x;
	UBYTE even_tiles[6U];
	UBYTE odd_tiles[6U];

	even_tiles[0] = 10U;
	even_tiles[1] = 12U;

	odd_tiles[0] = 9U;
	odd_tiles[1] = 11U;

	for(i = 0U; i != 20U; ++i) {
		disable_interrupts();
		if(select_scroll_dir == LEFT) x = i;
		else x = 19U - i;
		if(x & 1U) {
			set_bkg_tiles(x, 10U, 1U, 2U, odd_tiles);
			set_bkg_tiles(x, 12U, 1U, 2U, odd_tiles);
			set_bkg_tiles(x, 14U, 1U, 2U, odd_tiles);
		} else {
			set_bkg_tiles(x, 10U, 1U, 2U, even_tiles);
			set_bkg_tiles(x, 12U, 1U, 2U, even_tiles);
			set_bkg_tiles(x, 14U, 1U, 2U, even_tiles);
		}
		enable_interrupts();
		if(!(i & 1U)) {
			ticks++;
			if((ticks & 3U) == 3U) selectScrollCircles();
			if(select_scroll_dir == LEFT) select_arrow_offset1 = cos32_64[i >> 1];
			else select_arrow_offset2 = cos32_64[i >> 1];
			selectUpdateSprites();
			clearRemainingSprites();
			snd_update();
			wait_vbl_done();
		}
	}
	select_arrow_offset1 = 0U;
	select_arrow_offset2 = 0U;
}

void selectFadeIn() {
	UBYTE i, j, x;
	UBYTE *data;
	UBYTE *ptr;
	UBYTE tiles[6];

	disable_interrupts();
	data = selectGetBannerData();
	enable_interrupts();

	for(i = 0U; i != 20U; ++i) {
		if(select_scroll_dir == LEFT) x = i;
		else x = 19U - i;
		ptr = data + x;
		for(j = 0U; j != 6U; ++j) {
			tiles[j] = *ptr;
			ptr += 20U;
		}
		++ptr;

		disable_interrupts();
		set_bkg_tiles(x, 10U, 1U, 6U, tiles);
		enable_interrupts();
		if(i & 1U) {
			ticks++;
			if((ticks & 3U) == 3U) selectScrollCircles();
			if(select_scroll_dir == LEFT) select_arrow_offset1 = cos32_64[10U - (i >> 1)];
			else select_arrow_offset2 = cos32_64[10U - (i >> 1)];
			selectUpdateSprites();
			clearRemainingSprites();
			snd_update();
			wait_vbl_done();
		}
	}

	select_arrow_offset1 = 0U;
	select_arrow_offset2 = 0U;
}

void enterSelect() {
	UBYTE i, offset, name_index;
	initSelect();

	fadeFromWhite(6U);

	while(gamestate == GAMESTATE_SELECT) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			selectScrollCircles();
		}

		if(ISDOWN(J_RIGHT)) {
			selection++;
			select_scroll_dir = RIGHT;
			if(selection == 5U && levels_completed < 2U) selection++;
			if(selection > 6U) selection = 1U;
			playSound(SFX_MENU_SWITCH);
			selectFadeOut();
			selectFadeIn();
			selectUpdateSprites();
		}
		if(ISDOWN(J_LEFT)) {
			selection--;
			select_scroll_dir = LEFT;
			if(selection == 5U && levels_completed < 2U) selection--;
			if(selection == 0U) selection = 6U;
			playSound(SFX_MENU_SWITCH);
			selectFadeOut();
			selectFadeIn();
			selectUpdateSprites();
		}
		if(CLICKED(J_START) || CLICKED(J_A)) {
			if(selection == 5U) {
				gamestate = GAMESTATE_JUKEBOX;
				playSound(SFX_MENU_CONFIRM);
			} else if(selection == 6U) {
				gamestate = GAMESTATE_HIGHSCORE;
				playSound(SFX_MENU_CONFIRM);
			} else {
				if(selection <= levels_completed+1U) {
					level = selection;
					gamestate = GAMESTATE_INGAME;
					playSound(SFX_MENU_CONFIRM);
				} else {
					playSound(SFX_MENU_LOCKED);
				}
			}
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
			playSound(SFX_MENU_CANCEL);
		}

		// Draw level name
		if(selection <= 4U && selection > levels_completed+1U) {
			name_index = 0U;
		} else {
			name_index = selection;
		}

		offset = 64U;
		if(level_names[name_index][5] == 10U) {
			offset += 4U;
		}
		for(i = 0U; i != 6; ++i) {
			setSprite(offset+(i << 3), 69U+cos4_16[(i+(ticks >> 1)) & 15U], level_names[name_index][i], OBJ_PAL0);
		}

		selectUpdateSprites();
		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	stopMusic();
	clearRemainingSprites(); // Remove all sprites
	fadeToWhite(6U);
	wait_sound_done();
}
