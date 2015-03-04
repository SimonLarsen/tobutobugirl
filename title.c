#include <gb/gb.h>
#include "title.h"
#include "fade.h"
#include "gamestate.h"

#include "data/bg/title.h"

void initTitle() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, title_data_length, title_data);
	set_bkg_tiles(0U, 0U, title_tiles_width, title_tiles_height, title_tiles);
	
	HIDE_SPRITES;
	HIDE_WIN;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterTitle() {
	initTitle();

	fadeFromWhite(10U);

	while(1) {
		updateJoystate();
		if(CLICKED(J_START)) {
			gamestate = GAMESTATE_SELECT;
			break;
		}
		wait_vbl_done();
	}

	fadeToWhite(10U);
}
