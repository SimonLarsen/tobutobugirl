#include <gb/gb.h>
#include "defines.h"
#include "unlocked.h"
#include "fade.h"
#include "gamestate.h"

#include "data/sprite/characters.h"
#include "data/bg/circles.h"
#include "data/bg/unlocked.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"
#include "data/bg/selection5.h"

const UBYTE messages[2][24] = {
	{
		10U, 24U, 15U, 34U, 30U, 10U, 22U, 15U, 32U, 15U, 22U, 10U,
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U,
	},
	{
		23U, 31U, 29U, 19U, 13U, 10U, 26U, 22U, 11U, 35U, 15U, 28U,
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U,
	}
};


UBYTE unlocked_circle_index;

void initUnlocked() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data(0U, 38U, characters_data);
	set_bkg_data(38U, circles_data_length, circles_data);
	set_bkg_data(unlocked_offset, unlocked_data_length, unlocked_data);
	set_bkg_tiles(0U, 0U, unlocked_tiles_width, unlocked_tiles_height, unlocked_tiles);

	if(unlocked_bits & UNLOCKED_CLOUDS) {
		unlocked_bits ^= UNLOCKED_CLOUDS;
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection2_tiles);
	}
	else if(unlocked_bits & UNLOCKED_SPACE) {
		unlocked_bits ^= UNLOCKED_SPACE;
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection3_tiles);
	}
	else if(unlocked_bits & UNLOCKED_MUSIC) {
		unlocked_bits ^= UNLOCKED_MUSIC;
		set_bkg_data(selection5_offset, selection5_data_length, selection5_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection5_tiles);
	}

	BGP_REG = 0xE4U; // 11100100

	unlocked_circle_index = 0U;

	clearSprites();

	SHOW_SPRITES;
	HIDE_WIN;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterUnlocked() {
	initUnlocked();

	fadeFromWhite(6U);

	ticks = 0U;
	while(1U) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			unlocked_circle_index = (unlocked_circle_index+1U) & 7U;
			set_bkg_data(47U, 1U, &circles_data[(unlocked_circle_index << 4)]);
		}

		if(CLICKED(J_START) || CLICKED(J_A)) {
			if(unlocked_bits == 0U) {
				gamestate = GAMESTATE_HIGHSCORE;
			}
			break;
		}

		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(6U);

	stopMusic();
}
