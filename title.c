#include <gb/gb.h>
#include <rand.h>
#include "defines.h"
#include "title.h"
#include "fade.h"
#include "gamestate.h"

#include "data/bg/titlescreen.h"
#include "data/sprite/characters.h"

const UBYTE title_message[11] = {
	26U, 28U, 15U, 29U,
	29U, 10U, 29U, 30U,
	11U, 28U, 30U
};

void initTitle() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data_rle(0U, titlescreen_data_length, titlescreen_data);
	set_bkg_tiles_rle(0U, 0U, titlescreen_tiles_width, titlescreen_tiles_height, titlescreen_tiles);
	set_sprite_data(0U, 37U, characters_data);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

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
			initrand(DIV_REG);
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
