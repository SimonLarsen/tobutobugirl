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

#include "data/bg/circles.h"
#include "data/bg/select.h"

#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"
#include "data/bg/selection4.h"
#include "data/bg/selection_highscore.h"
#include "data/bg/selection_jukebox.h"
#include "data/bg/selection_locked.h"

UBYTE select_circle_index;
UBYTE select_ticks;

extern UBYTE mainmenu_song_data;

void initSelect() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_sprite_data(0U, 37U, characters_data);
	set_sprite_data(37U, arrow_data_length, arrow_data);

	set_bkg_data(0U, circles_data_length, circles_data);
	set_bkg_data_rle(select_offset, select_data_length, select_data);
	set_bkg_tiles_rle(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

	select_circle_index = 0U;
	ticks = 0U;

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xB4U; // 11100100
	BGP_REG = 0xB4U; // 11100100

	clearSprites();
	_selectUpdateScreen();

	setMusicBank(4U);
	playMusic(&mainmenu_song_data);

	HIDE_WIN;
	SPRITES_8x8;
	SHOW_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

UBYTE *selectGetBannerData() {
	if(selection == 0U) {
		set_bkg_data(selection_highscore_offset, selection_highscore_data_length, selection_highscore_data);
		return selection_highscore_tiles;
	} else if(selection == 5U) {
		set_bkg_data(selection_jukebox_offset, selection_jukebox_data_length, selection_jukebox_data);
		return selection_jukebox_tiles;
	} else if(selection > levels_completed+1U) {
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
	}
	return 0U;
}

void selectUpdateScreen() {
	clearRemainingSprites();
	fadeToWhite(4U);
	DISPLAY_OFF;

	_selectUpdateScreen();

	DISPLAY_ON;
	fadeFromWhite(4U);
}

void _selectUpdateScreen() {
	UBYTE *data = selectGetBannerData();
	set_bkg_tiles(0U, 10U, 20U, 6U, data);
}

void selectUpdateSprites() {
	UBYTE offset;

	offset = cos32_64[(ticks & 63U)] >> 3;

	setSprite(24U-offset, 61U, 37U, OBJ_PAL0);
	setSprite(32U-offset, 61U, 39U, OBJ_PAL0);
	setSprite(24U-offset, 69U, 38U, OBJ_PAL0);
	setSprite(32U-offset, 69U, 40U, OBJ_PAL0);

	setSprite(136U+offset, 61U, 39U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+offset, 61U, 37U, OBJ_PAL0 | FLIP_X);
	setSprite(136U+offset, 69U, 40U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+offset, 69U, 38U, OBJ_PAL0 | FLIP_X);
}

void selectScrollCircles() {
	select_circle_index = (select_circle_index+1U) & 7U;
	set_bkg_data(30U, 1U, &circles_data[(select_circle_index << 4)]);
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
			if(selection == 4U && levels_completed < 2U) selection = 0U;
			else if(selection == 5U) selection = 0U;
			else selection++;
			selectUpdateScreen();
			selectUpdateSprites();
		}
		if(ISDOWN(J_LEFT)) {
			if(selection == 0U) {
				if(levels_completed >= 2U) {
					selection = 5U;
				} else {
					selection = 4U;
				}
			}
			else selection--;
			selectUpdateScreen();
			selectUpdateSprites();
		}
		if(CLICKED(J_START) || CLICKED(J_A)) {
			if(selection == 0U) {
				gamestate = GAMESTATE_HIGHSCORE;
			} else if(selection == 5U) {
				gamestate = GAMESTATE_JUKEBOX;
			} else if(selection <= levels_completed+1U) { // TODO: Remove cheat again
				level = selection;
				gamestate = GAMESTATE_INGAME;
			}
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
		}

		// Draw level name
		if(selection == 0U || selection == 5U
		|| (selection <= levels_completed+1U)) {
			name_index = selection;
		} else {
			name_index = 6U;
		}
		offset = 64U;
		if(level_names[name_index][5] == 10U) {
			offset += 4U;
		}
		for(i = 0U; i != 6; ++i) {
			setSprite(offset+(i << 3), 61U+cos4_16[(i+(ticks >> 1)) & 15U], level_names[name_index][i], OBJ_PAL0);
		}

		selectUpdateSprites();
		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites(); // Remove all sprites
	fadeToWhite(10U);

	stopMusic();
}
