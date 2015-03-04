#include <gb/gb.h>
#include "select.h"
#include "fade.h"
#include "gamestate.h"

#include "data/sprite/characters.h"

#include "data/bg/circles.h"
#include "data/bg/select.h"

#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"

UBYTE select_circle_index;

void initSelect() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, circles_data_length, circles_data);
	set_bkg_data(select_offset, select_data_length, select_data);
	set_bkg_tiles(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

	select_circle_index = 0U;
	ticks = 0U;

	updateSelectScreen();

	HIDE_SPRITES;
	HIDE_WIN;
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

void setCharacter(UBYTE x, UBYTE y, UBYTE c) {
	set_bkg_tiles(x, y, 1U, 1U, &c);
	/*
	if(c <= 10U) {
		set_bkg_tiles(x, y, 1U, 1U, &c);
	} else {
		set_bkg_tiles(x, y, 1U, 1U, &c);
	}
	*/
}

void selectTransitionOut() {
	UBYTE tile = 8U;
	UBYTE ix, iy;
	UBYTE top, bottom, left, right;
	top = 10U;
	bottom = 15U;
	left = 0U;
	right = 19U;

	/*
	for(ix = 0U; ix != 8U; ++ix) {
		setCharacter(ix+6U, 6U, select_names[level][ix]);
	}
	*/

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
	initSelect();

	fadeFromWhite(10U);

	while(1) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			scrollCircles();
		}

		if(CLICKED(J_RIGHT)) {
			level++;
			if(level == 4U) level = 1U;
			selectTransitionOut();
			selectTransitionIn();
		}
		if(CLICKED(J_LEFT)) {
			level--;
			if(level == 0U) level = 3U;
			selectTransitionOut();
			selectTransitionIn();
		}
		if(CLICKED(J_START) || CLICKED(J_A)) {
			gamestate = GAMESTATE_INGAME;
			break;
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
			break;
		}

		wait_vbl_done();
	}

	fadeToWhite(10U);
}
