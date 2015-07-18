#include <gb/gb.h>
#include "gamestate.h"
#include "fade.h"
#include "winscreen.h"

#include "data/sprite/characters.h"
#include "data/bg/win_base.h"
#include "data/bg/win1.h"
#include "data/bg/win2.h"
#include "data/bg/win3.h"

void drawScore(UBYTE x, UBYTE y, UBYTE value) {
	UBYTE tile;

	if(value) {
		if(value >= 10U) {
			tile = value / 10U;
			set_bkg_tiles(x, y, 1U, 1U, &tile);
		}
		tile = value % 10U;
		set_bkg_tiles(x+1U, y, 1U, 1U, &tile);
		tile = 0U;
		set_bkg_tiles(x+2U, y, 1U, 1U, &tile);
	}
	tile = 0U;
	set_bkg_tiles(x+3U, y, 1U, 1U, &tile);
}

void countUpScore(UBYTE x, UBYTE y, UBYTE value, UBYTE delay_time) {
	UBYTE i;
	for(i = 0U; i != value+1U; ++i) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B)) {
			i = value;
		}
		drawScore(x, y, i);
		delay(delay_time);
	}
}

void initWinscreen() {
	UBYTE *data;

	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	clearSprites();
	BGP_REG = 0xE4U; // 11100100

	set_bkg_data(0U, 40U, characters_data);
	set_bkg_data(win_base_offset, win_base_data_length, win_base_data);
	set_bkg_tiles(0U, 0U, win_base_tiles_width, win_base_tiles_height, win_base_tiles);

	switch(level) {
		case 1:
			set_bkg_data_rle(win1_offset, win1_data_length, win1_data);
			set_bkg_tiles_rle(10U, 5U, win1_tiles_width, win1_tiles_height, win1_tiles);
			break;
		case 2:
			set_bkg_data_rle(win2_offset, win2_data_length, win2_data);
			set_bkg_tiles_rle(8U, 3U, win2_tiles_width, win2_tiles_height, win2_tiles);
			break;
		case 3:
			set_bkg_data_rle(win3_offset, win3_data_length, win3_data);
			set_bkg_tiles_rle(10U, 3U, win3_tiles_width, win3_tiles_height, win3_tiles);
			break;
	}

	// Set level name
	data = level_names[level];
	set_bkg_tiles(4U+(level==3U), 1U, 6U, 1U, data);

	HIDE_SPRITES;
	HIDE_WIN;
	SHOW_BKG;
	DISPLAY_ON;
	enable_interrupts();
}

void enterWinscreen() {
	UBYTE tile, tmp;
	initWinscreen();

	fadeFromWhite(10U);

	delay(255U);

	// Time
	tile = elapsed_time / 60U;
	set_bkg_tiles(1U, 5U, 1U, 1U, &tile);
	tile = 37U;
	set_bkg_tiles(2U, 5U, 1U, 1U, &tile);
	tile = (elapsed_time % 60U) / 10U;
	set_bkg_tiles(3U, 5U, 1U, 1U, &tile);
	tile = (elapsed_time % 60U) % 10U;
	set_bkg_tiles(4U, 5U, 1U, 1U, &tile);

	delay(512U);

	// Count up time bonus
	countUpScore(4, 6, 2U*remaining_time, 80U);

	delay(512U);

	// Kills
	tmp = kills;
	tile = tmp / 10U;
	set_bkg_tiles(1U, 10U, 1U, 1U, &tile);
	tile = tmp % 10U;
	set_bkg_tiles(2U, 10U, 1U, 1U, &tile);

	delay(512U);

	// Count up kill bonus
	countUpScore(4U, 11U, kills, 30U);

	delay(512U);

	// Count up total score
	countUpScore(4U, 15U, 2U*remaining_time+kills, 30U);

	while(1) {
		updateJoystate();
		if(CLICKED(J_START) || CLICKED(J_A)) {
			gamestate = GAMESTATE_HIGHSCORE;
			break;
		}
		wait_vbl_done();
	}

	fadeToWhite(10U);
}
