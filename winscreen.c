#include <gb/gb.h>
#include "gamestate.h"
#include "fade.h"
#include "winscreen.h"

#include "data/bg/win1.h"
#include "data/bg/win2.h"
#include "data/bg/win3.h"

void initWinscreen() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	switch(level) {
		case 1:
			set_bkg_data(0U, win1_data_length, win1_data);
			set_bkg_tiles(0U, 0U, win1_tiles_width, win1_tiles_height, win1_tiles);
			break;
		case 2:
			set_bkg_data(0U, win2_data_length, win2_data);
			set_bkg_tiles(0U, 0U, win2_tiles_width, win2_tiles_height, win2_tiles);
			break;
		case 3:
			set_bkg_data(0U, win3_data_length, win3_data);
			set_bkg_tiles(0U, 0U, win3_tiles_width, win3_tiles_height, win3_tiles);
			break;
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
