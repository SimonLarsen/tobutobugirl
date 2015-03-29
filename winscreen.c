#include <gb/gb.h>
#include "gamestate.h"
#include "fade.h"
#include "winscreen.h"

#include "data/bg/winscreen.h"

void initWinscreen() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, winscreen_data_length, winscreen_data);
	set_bkg_tiles(0U, 0U, winscreen_tiles_width, winscreen_tiles_height, winscreen_tiles);

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
