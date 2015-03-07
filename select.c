#include <gb/gb.h>
#include "defines.h"
#include "binconst.h"
#include "select.h"
#include "fade.h"
#include "gamestate.h"
#include "cos.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"

#include "data/bg/circles.h"
#include "data/bg/select.h"

#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"

UBYTE select_circle_index;

const UBYTE level_names[3][6] = {
	{26U, 22U, 11U, 19U, 24U, 29U},
	{13U, 22U, 25U, 31U, 14U, 29U},
	{29U, 26U, 11U, 13U, 15U, 10U}
};

void initSelect() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_sprite_data(0U, characters_data_length, characters_data);
	set_sprite_data(37U, arrow_data_length, arrow_data);
	set_bkg_data(0U, circles_data_length, circles_data);
	set_bkg_data(select_offset, select_data_length, select_data);
	set_bkg_tiles(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

	select_circle_index = 0U;
	ticks = 0U;

	OBP0_REG = B8(11010000);
	OBP1_REG = B8(11100100);
	BGP_REG = B8(11100100);

	clearSprites();
	updateSelectScreen();

	HIDE_WIN;
	SPRITES_8x8;
	SHOW_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void setTile(UBYTE x, UBYTE y, UBYTE *tile) {
	set_bkg_tiles(x, y, 1U, 1U, tile);
	ticks++;
	if((ticks & 14U) == 14U) {
		ticks = 0U;
		scrollCircles();
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

	if(level == 1U) {
		set_bkg_data(selection1_offset, selection1_data_length, selection1_data);
		data = selection1_tiles;
	} else if(level == 2U) {
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		data = selection2_tiles;
	} else if(level == 3U) {
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
	if(level == 1U) {
		set_bkg_data(selection1_offset, selection1_data_length, selection1_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection1_tiles);
	} else if(level == 2U) {
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection2_tiles);
	} else if(level == 3U) {
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		set_bkg_tiles(0U, 10U, 20U, 6U, selection3_tiles);
	}
}

void scrollCircles() {
	select_circle_index = (select_circle_index+1U) & 7U;
	set_bkg_data(34U, 1U, &circles_data[(select_circle_index << 4)]);
}

void enterSelect() {
	UBYTE i, offset;
	initSelect();

	fadeFromWhite(10U);

	while(gamestate == GAMESTATE_SELECT) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			scrollCircles();
		}

		if(CLICKED(J_RIGHT)) {
			level++;
			if(level == 4U) level = 1U;
			clearRemainingSprites();
			selectTransitionOut();
			selectTransitionIn();
		}
		if(CLICKED(J_LEFT)) {
			level--;
			if(level == 0U) level = 3U;
			clearRemainingSprites();
			selectTransitionOut();
			selectTransitionIn();
		}
		if(CLICKED(J_START) || CLICKED(J_A)) {
			gamestate = GAMESTATE_INGAME;
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
		}

		// Draw level name
		offset = 64U;
		if(level == 3U) {
			offset += 4U;
		}
		for(i = 0U; i != 6; ++i) {
			setSprite(offset+(i << 3), 61U+cos4_16[(i+(ticks >> 1)) & 15U], level_names[level-1U][i], OBJ_PAL0);
		}

		// Draw arrows
		offset = cos32_64[(ticks & 63U)] >> 3;

		setSprite(24U-offset, 60U, 37U, OBJ_PAL0);
		setSprite(32U-offset, 60U, 38U, OBJ_PAL0);
		setSprite(24U-offset, 68U, 39U, OBJ_PAL0);
		setSprite(32U-offset, 68U, 40U, OBJ_PAL0);

		setSprite(136U+offset, 60U, 38U, OBJ_PAL0 | FLIP_X);
		setSprite(144U+offset, 60U, 37U, OBJ_PAL0 | FLIP_X);
		setSprite(136U+offset, 68U, 40U, OBJ_PAL0 | FLIP_X);
		setSprite(144U+offset, 68U, 39U, OBJ_PAL0 | FLIP_X);

		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites(); // Remove all sprites
	fadeToWhite(10U);
}
