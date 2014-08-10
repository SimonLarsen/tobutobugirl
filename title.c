#include <gb/gb.h>
#include "binconst.h"
#include "input.h"
#include "gamestate.h"
#include "fade.h"

#include "title.h"

#include "data/bg/title.h"

void enterTitle() {
	UBYTE loop;

	disable_interrupts();
	DISPLAY_OFF;

	BGP_REG = B8(11100100);

	set_bkg_data(0U, title_data_length, title_data);
	set_bkg_tiles(0U, 0U, title_tiles_width, title_tiles_height, title_tiles);

	SHOW_BKG;
	HIDE_SPRITES;

	DISPLAY_ON;
	enable_interrupts();

	loop = 1U;
	while(loop) {
		updateJoystate();
		if(CLICKED(J_START)) {
			gamestate = GAMESTATE_GAME;
			loop = 0U;
		}
		wait_vbl_done();
	}

	fadeToWhite();
}
