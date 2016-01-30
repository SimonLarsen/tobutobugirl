#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "sound.h"

#include "data/sprite/characters.h"
#include "data/bg/pause_bg.h"

const UBYTE pause_text_on[] = { 10U, 25U, 24U };
const UBYTE pause_text_off[] = { 25U, 16U, 16U };

const UBYTE pause_marker_x[] = { 48U, 8U, 56U };
const UBYTE pause_marker_y[] = { 112U, 124U, 136U };

UBYTE pause_selection;

void initPause() {
	disable_interrupts();
	DISPLAY_OFF;

	set_bkg_data(0, 38U, characters_data);
	set_bkg_data_rle(pause_bg_offset, pause_bg_data_length, pause_bg_data);
	set_bkg_tiles_rle(0U, 0U, pause_bg_tiles_width, pause_bg_tiles_height, pause_bg_tiles);

	if(show_dashcounter) {
		set_bkg_tiles(15U, 14U, 3U, 1U, pause_text_on);
	} else {
		set_bkg_tiles(15U, 14U, 3U, 1U, pause_text_off);
	}

	move_bkg(0U, 0U);

	SHOW_BKG;
	SHOW_SPRITES;
	HIDE_WIN;

	DISPLAY_ON;
	enable_interrupts();
}

UBYTE enterPause() {
	clearRemainingSprites();
	initPause();

	pause_selection = 0U;

	while(1U) {
		updateJoystate();
		
		if(CLICKED(J_START)) {
			return 0U;
		}

		if(CLICKED(J_UP)) {
			if(pause_selection == 0U) pause_selection = 3U;
			pause_selection--;
		}
		if(CLICKED(J_DOWN)) {
			pause_selection++;
			if(pause_selection == 3U) pause_selection = 0U;
		}
		if(CLICKED(J_A)) {
			if(pause_selection == 0U) {
				return 0U;
			} else if(pause_selection == 1U) {
				show_dashcounter = !show_dashcounter;
				disable_interrupts();
				if(show_dashcounter) {
					set_bkg_tiles(15U, 14U, 3U, 1U, pause_text_on);
				} else {
					set_bkg_tiles(15U, 14U, 3U, 1U, pause_text_off);
				}
				enable_interrupts();
			} else if(pause_selection == 2U) {
				return 3U;
			}
		}

		setSprite(pause_marker_x[pause_selection], pause_marker_y[pause_selection], 104U, OBJ_PAL0);
		setSprite(pause_marker_x[pause_selection]+8U, pause_marker_y[pause_selection], 106U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}
}
