#include <gb/gb.h>
#include "defines.h"
#include "binconst.h"
#include "title.h"
#include "fade.h"
#include "gamestate.h"

#include "data/sprite/characters.h"
#include "data/bg/title.h"

const UBYTE title_message[11] = {
	26U, 28U, 15U, 29U,
	29U, 10U, 29U, 30U,
	11U, 28U, 30U
};

void initTitle() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_sprite_data(0U, 37U, characters_data);

	set_bkg_data(0U, title_data_length, title_data);
	set_bkg_tiles(0U, 0U, title_tiles_width, title_tiles_height, title_tiles);
	
	OBP0_REG = B8(11010000);
	BGP_REG = B8(11100100);

	clearSprites();

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterTitle() {
	UBYTE i;

	initTitle();

	fadeFromWhite(10U);

	selection = level = 1U;

	ticks = 0U;
	while(1) {
		updateJoystate();
		if(CLICKED(J_START)) {
			gamestate = GAMESTATE_SELECT;
			break;
		}

		if(ticks < 80U) {
			for(i = 0U; i != 11U; ++i) {
				if(i != 5U) {
					setSprite(48U + (i << 3), 136U, title_message[i], OBJ_PAL0);
				}
			}
		}
		ticks++;
		if(ticks == 100U) ticks = 0U;

		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(10U);
}
