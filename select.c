#include <gb/gb.h>
#include "defines.h"
#include "select.h"
#include "fade.h"
#include "gamestate.h"
#include "cos.h"
#include "ram.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"

#include "data/bg/circles.h"
#include "data/bg/select.h"

#include "data/bg/selection0.h"
#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"
#include "data/bg/selection4.h"

UBYTE select_circle_index;
UBYTE select_ticks;
UBYTE completed;

extern UBYTE mainmenu_song_data;

void initSelect() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_sprite_data(0U, 37U, characters_data);
	set_sprite_data(37U, arrow_data_length, arrow_data);
	set_bkg_data(0U, circles_data_length, circles_data);

	set_bkg_data(select_offset, select_data_length, select_data);
	set_bkg_tiles(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

	select_circle_index = 0U;
	ticks = 0U;

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xB4U; // 11100100
	BGP_REG = 0xB4U; // 11100100

	clearSprites();
	updateSelectScreen();

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	for(completed = 0U; completed != 3U; ++completed) {
		if(ram_data[completed << 4] == 0U) break;
	}

	DISABLE_RAM_MBC1;

	setMusicBank(4U);
	playMusic(&mainmenu_song_data);

	HIDE_WIN;
	SPRITES_8x8;
	SHOW_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void setTile(UBYTE x, UBYTE y, UBYTE *tile) {
	set_bkg_tiles(x, y, 1U, 1U, tile);
	select_ticks++;
	if((select_ticks & 14U) == 14U) {
		select_ticks = 0U;
		selectScrollCircles();
		selectUpdateSprites();
		clearRemainingSprites();
	}
	delay(3U);
}

void selectTransitionOut() {
	UBYTE tile = 8U;
	UBYTE ix, iy;
	UBYTE top, bottom, left, right;
	top = 10U;
	bottom = 15U;
	left = 0U;
	right = 19U;

	do {
		ix = left;
		iy = top;
		while(ix != right) {
			setTile(ix, iy, &tile);
			ix++;
		}
		while(iy != bottom) {
			setTile(ix, iy, &tile);
			iy++;
		}
		while(ix != left) {
			setTile(ix, iy, &tile);
			ix--;
		}
		while(iy != top) {
			setTile(ix, iy, &tile);
			iy--;
		}
		bottom--;
		right--;
		top++;
		left++;
	} while(top <= bottom);
}

void selectTransitionIn() {
	UBYTE *tile, *data;
	UBYTE ix, iy;
	UBYTE top, bottom, left, right;
	top = 10U;
	bottom = 15U;
	left = 0U;
	right = 19U;

	if(selection > completed+1U) {
		set_bkg_data(selection4_offset, selection4_data_length, selection4_data);
		data = selection4_tiles;
	} else if(selection == 0U) {
		set_bkg_data(selection0_offset, selection0_data_length, selection0_data);
		data = selection0_tiles;
	} else if(selection == 1U) {
		set_bkg_data(selection1_offset, selection1_data_length, selection1_data);
		data = selection1_tiles;
	} else if(selection == 2U) {
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		data = selection2_tiles;
	} else if(selection == 3U) {
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		data = selection3_tiles;
	}

	tile = data;
	do {
		ix = left;
		iy = top;
		while(ix != right) {
			setTile(ix, iy, tile);
			ix++;
			tile++;
		}
		while(iy != bottom) {
			setTile(ix, iy, tile);
			iy++;
			tile += 20U;
		}
		while(ix != left) {
			setTile(ix, iy, tile);
			ix--;
			tile--;
		}
		while(iy != top) {
			setTile(ix, iy, tile);
			iy--;
			tile -= 20U;
		}
		bottom--;
		right--;
		top++;
		left++;
		tile += 21U;
	} while(top <= bottom);
}

void updateSelectScreen() {
	if(selection == 0U) {
		set_bkg_data(selection0_offset, selection0_data_length, selection0_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection0_tiles);
	} else if(selection > completed+1U) {
		set_bkg_data(selection4_offset, selection4_data_length, selection4_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection4_tiles);
	} else if(selection == 1U) {
		set_bkg_data(selection1_offset, selection1_data_length, selection1_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection1_tiles);
	} else if(selection == 2U) {
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection2_tiles);
	} else if(selection == 3U) {
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection3_tiles);
	}
}

void selectUpdateSprites() {
	UBYTE offset;

	offset = cos32_64[(ticks & 63U)] >> 3;

	setSprite(24U-offset, 61U, 37U, OBJ_PAL0);
	setSprite(32U-offset, 61U, 38U, OBJ_PAL0);
	setSprite(24U-offset, 69U, 39U, OBJ_PAL0);
	setSprite(32U-offset, 69U, 40U, OBJ_PAL0);

	setSprite(136U+offset, 61U, 38U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+offset, 61U, 37U, OBJ_PAL0 | FLIP_X);
	setSprite(136U+offset, 69U, 40U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+offset, 69U, 39U, OBJ_PAL0 | FLIP_X);
}

void selectScrollCircles() {
	select_circle_index = (select_circle_index+1U) & 7U;
	set_bkg_data(30U, 1U, &circles_data[(select_circle_index << 4)]);
}

void enterSelect() {
	UBYTE i, offset, name_index;
	initSelect();

	fadeFromWhite(10U);

	while(gamestate == GAMESTATE_SELECT) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			selectScrollCircles();
		}

		if(CLICKED(J_RIGHT)) {
			if(selection == 3U) selection = 0U;
			else selection++;
			selectTransitionOut();
			selectTransitionIn();
			selectUpdateSprites();
		}
		if(CLICKED(J_LEFT)) {
			if(selection == 0U) selection = 3U;
			else selection--;
			selectTransitionOut();
			selectTransitionIn();
			selectUpdateSprites();
		}
		if(CLICKED(J_START) || CLICKED(J_A)) {
			if(selection == 0U) {
				gamestate = GAMESTATE_HIGHSCORE;
			} else if(selection <= completed+1U) {
				level = selection;
				gamestate = GAMESTATE_INGAME;
			}
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
		}

		// Draw level name
		if(selection <= completed+1U) {
			name_index = selection;
		} else {
			name_index = 4U;
		}
		offset = 64U;
		if(name_index == 3U) {
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
}
