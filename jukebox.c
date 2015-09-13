#include <gb/gb.h>
#include "jukebox.h"
#include "fade.h"
#include "gamestate.h"
#include "data/bg/jukebox.h"

void initJukebox() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data_rle(0U, jukebox_data_length, jukebox_data);
	set_bkg_tiles_rle(0U, 0U, jukebox_tiles_width, jukebox_tiles_height, jukebox_tiles);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	clearSprites();

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	DISPLAY_ON;
	enable_interrupts();
}

void enterJukebox() {
	initJukebox();

	fadeFromWhite(10U);

	while(1U) {
		updateJoystate();

		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
			break;
		}

		clearRemainingSprites();
		wait_vbl_done();
	}

	fadeToWhite(10U);
}
