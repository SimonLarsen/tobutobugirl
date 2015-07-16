#include <gb/gb.h>
#include "gamestate.h"
#include "fade.h"
#include "winscreen.h"

#include "data/sprite/characters.h"
#include "data/bg/win_base.h"
#include "data/bg/win1.h"
#include "data/bg/win2.h"
#include "data/bg/win3.h"

void initWinscreen() {
	UBYTE tile, tmp;
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
			set_bkg_tiles_rle(8U, 3U, win1_tiles_width, win1_tiles_height, win1_tiles);
			break;
		case 2:
			set_bkg_data_rle(win2_offset, win2_data_length, win2_data);
			set_bkg_tiles_rle(8U, 3U, win2_tiles_width, win2_tiles_height, win2_tiles);
			break;
		case 3:
			set_bkg_data_rle(win3_offset, win3_data_length, win3_data);
			set_bkg_tiles_rle(8U, 3U, win3_tiles_width, win3_tiles_height, win3_tiles);
			break;
	}

	// Set level name
	data = level_names[level];
	set_bkg_tiles(4U+(level==3U), 1U, 6U, 1U, data);

	// Time
	tile = elapsed_time / 60U;
	set_bkg_tiles(1U, 5U, 1U, 1U, &tile);
	tile = (elapsed_time % 60U) / 10U;
	set_bkg_tiles(3U, 5U, 1U, 1U, &tile);
	tile = (elapsed_time % 60U) % 10U;
	set_bkg_tiles(4U, 5U, 1U, 1U, &tile);
	// Time bonus
	tmp = remaining_time;
	if(tmp) {
		tile = tmp / 10U;
		if(tile) set_bkg_tiles(4U, 6U, 1U, 1U, &tile);
		tile = tmp % 10U;
		set_bkg_tiles(5U, 6U, 1U, 1U, &tile);
		tile = 0U;
		set_bkg_tiles(6U, 6U, 1U, 1U, &tile);
	}
	// Kills
	tmp = kills;
	tile = tmp / 10U;
	set_bkg_tiles(1U, 10U, 1U, 1U, &tile);
	tile = tmp % 10U;
	set_bkg_tiles(2U, 10U, 1U, 1U, &tile);
	// Kill bonus
	tmp = 2U * kills;
	if(tmp) {
		tile = tmp / 10U;
		if(tile) set_bkg_tiles(4U, 11U, 1U, 1U, &tile);
		tile = tmp % 10U;
		set_bkg_tiles(5U, 11U, 1U, 1U, &tile);
		tile = 0U;
		set_bkg_tiles(6U, 11U, 1U, 1U, &tile);
	}

	tmp = remaining_time + 2U*kills;
	if(tmp) {
		tile = tmp / 10U;
		if(tile) set_bkg_tiles(4U, 15U, 1U, 1U, &tile);
		tile = tmp % 10U;
		set_bkg_tiles(5U, 15U, 1U, 1U, &tile);
		tile = 0U;
		set_bkg_tiles(6U, 15U, 1U, 1U, &tile);
	}

	HIDE_SPRITES;
	HIDE_WIN;
	SHOW_BKG;
	DISPLAY_ON;
	enable_interrupts();
}

void enterWinscreen() {
	initWinscreen();

	fadeFromWhite(10U);

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
