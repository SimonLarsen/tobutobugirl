#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "sound.h"

#include "data/sprite/characters.h"

const UBYTE pause_text_on[] = { 10U, 25U, 24U };
const UBYTE pause_text_off[] = { 25U, 16U, 16U };

const UBYTE pause_marker_x1[] = { 49U, 9U, 57U };
const UBYTE pause_marker_x2[] = { 111U, 150U, 102U };
const UBYTE pause_marker_y[] = { 112U, 124U, 136U };

UBYTE pause_selection;

void pauseUpdateDashCounter() {
	if(show_dashcounter) {
		set_bkg_tiles(15U, 14U, 3U, 1U, pause_text_on);
	} else {
		set_bkg_tiles(15U, 14U, 3U, 1U, pause_text_off);
	}
}

void initPause() {
	disable_interrupts();
	DISPLAY_OFF;

	set_bkg_data(0, 38U, characters_data);
	setIngameBackground(255U);

	pauseUpdateDashCounter();

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
				pauseUpdateDashCounter();
				enable_interrupts();
			} else if(pause_selection == 2U) {
				return 3U;
			}
		}

		setSprite(pause_marker_x1[pause_selection], pause_marker_y[pause_selection], 104U, OBJ_PAL0);
		setSprite(pause_marker_x1[pause_selection]+8U, pause_marker_y[pause_selection], 106U, OBJ_PAL0);

		setSprite(pause_marker_x2[pause_selection], pause_marker_y[pause_selection], 108U, OBJ_PAL0);
		setSprite(pause_marker_x2[pause_selection]+8U, pause_marker_y[pause_selection], 110U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}
}
