#include <gb/gb.h>
#include "defines.h"
#include "unlocked.h"
#include "fade.h"
#include "gamestate.h"

#include "data/sprite/characters.h"
#include "data/bg/zoom_circles.h"
#include "data/bg/unlocked.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"
#include "data/bg/selection4.h"
#include "data/bg/selection_jukebox.h"

extern UBYTE unlocked_song_data;

const UBYTE unlocked_messages[3][24] = {
	{
		10U, 24U, 15U, 34U, 30U, 10U, 22U, 15U, 32U, 15U, 22U, 10U, // " NEXT LEVEL "
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U  // "  UNLOCKED  "
	},
	{
		23U, 31U, 29U, 19U, 13U, 10U, 26U, 22U, 11U, 35U, 15U, 28U, // "MUSIC PLAYER"
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U  // "  UNLOCKED  "
	},
	{
		10U, 10U, 10U, 14U, 28U, 15U, 11U, 23U, 10U, 10U, 10U, 10U, // "   DREAM    "
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U  // "  UNLOCKED  "
	}
};


UBYTE unlocked_circle_index;

void initUnlocked() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data(0U, 38U, characters_data);
	set_bkg_data(38U, zoom_circles_data_length, zoom_circles_data);
	set_bkg_data(unlocked_offset, unlocked_data_length, unlocked_data);
	set_bkg_data(47U, 8U, &zoom_circles_data[0]);

	set_bkg_tiles(0U, 0U, unlocked_tiles_width, unlocked_tiles_height, unlocked_tiles);

	if(unlocked_bits & UNLOCKED_CLOUDS) {
		unlocked_bits ^= UNLOCKED_CLOUDS;
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection2_tiles);
		set_bkg_tiles(4U, 5U, 12U, 2U, unlocked_messages[0]);
	}
	else if(unlocked_bits & UNLOCKED_SPACE) {
		unlocked_bits ^= UNLOCKED_SPACE;
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection3_tiles);
		set_bkg_tiles(4U, 5U, 12U, 2U, unlocked_messages[0]);
	}
	else if(unlocked_bits & UNLOCKED_MUSIC) {
		unlocked_bits ^= UNLOCKED_MUSIC;
		set_bkg_data(selection_jukebox_offset, selection_jukebox_data_length, selection_jukebox_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection_jukebox_tiles);
		set_bkg_tiles(4U, 5U, 12U, 2U, unlocked_messages[1]);
	}
	else if(unlocked_bits & UNLOCKED_DREAM) {
		unlocked_bits ^= UNLOCKED_DREAM;
		set_bkg_data(selection4_offset, selection4_data_length, selection4_data);
		set_bkg_tiles(0U, 8U, 20U, 6U, selection4_tiles);
		set_bkg_tiles(4U, 5U, 12U, 2U, unlocked_messages[2]);
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

	setMusicBank(9U);
	disable_interrupts();
	playMusic(&unlocked_song_data);
	enable_interrupts();

	ticks = 0U;
	while(1U) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			unlocked_circle_index = (unlocked_circle_index+1U) & 7U;
			set_bkg_data(47U+unlocked_circle_index, 8U-unlocked_circle_index, &zoom_circles_data[0]);
			if(unlocked_circle_index) {
				set_bkg_data(47U, unlocked_circle_index, &zoom_circles_data[(8U-unlocked_circle_index) << 4]);
			}
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
