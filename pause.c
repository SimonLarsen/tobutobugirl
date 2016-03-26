#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "sound.h"
#include "ram.h"

#include "data/sprite/characters.h"

const UBYTE pause_text_on[] = { 10U, 25U, 24U };
const UBYTE pause_text_off[] = { 25U, 16U, 16U };

const UBYTE pause_marker_x1[] = { 49U, 17U, 57U };
const UBYTE pause_marker_x2[] = { 110U, 142U, 102U };
const UBYTE pause_marker_y[] = { 112U, 124U, 136U };

UBYTE pause_selection;
UBYTE pause_ticks;

void pauseUpdateDashCounter() {
	if(show_dashcounter) {
		set_bkg_tiles(14U, 14U, 3U, 1U, pause_text_on);
	} else {
		set_bkg_tiles(14U, 14U, 3U, 1U, pause_text_off);
	}
}

void initPause() {
	disable_interrupts();
	DISPLAY_OFF;

	set_bkg_data(0, 38U, characters_data);
	setIngameBackground(255U);
	setCloudAnimation(player_skin);

	pauseUpdateDashCounter();

	move_bkg(0U, 0U);

	SHOW_BKG;
	SHOW_SPRITES;
	HIDE_WIN;

	DISPLAY_ON;
	enable_interrupts();
}

UBYTE enterPause() {
	UBYTE i, j, frame;

	clearRemainingSprites();
	initPause();

	pause_selection = 0U;
	pause_ticks = 0U;

	while(1U) {
		pause_ticks++;
		updateJoystate();
		
		if(CLICKED(J_START)) {
			return 0U;
		}

		if(CLICKED(J_UP)) {
			if(pause_selection != 0) {
				pause_selection--;
				playSound(SFX_MENU_SWITCH);
			}
		}
		if(CLICKED(J_DOWN)) {
			if(pause_selection != 2U) {
				pause_selection++;
				playSound(SFX_MENU_SWITCH);
			}
		}
		if(CLICKED(J_A) || (pause_selection == 1U && (CLICKED(J_LEFT) || CLICKED(J_RIGHT)))) {
			playSound(SFX_MENU_CONFIRM);
			if(pause_selection == 0U) {
				wait_sound_done();
				return 0U;
			} else if(pause_selection == 1U) {
				ENABLE_RAM_MBC1;
				SWITCH_RAM_MBC1(0);
				show_dashcounter = !show_dashcounter;
				ram_data[RAM_DASHCOUNTER] = !ram_data[RAM_DASHCOUNTER];
				DISABLE_RAM_MBC1;

				disable_interrupts();
				pauseUpdateDashCounter();
				enable_interrupts();
			} else if(pause_selection == 2U) {
				wait_sound_done();
				return 3U;
			}
		}

		// Draw cloud animation
		frame = pause_ticks & 48U;
		for(j = 0U; j != 2U; ++j) {
			for(i = 0U; i != 4U; ++i) {
				setSprite(72U+(i << 3U), 48U+(j << 4U), frame, OBJ_PAL0);
				frame += 2U;
			}
		}

		// Draw selection markers
		setSprite(pause_marker_x1[pause_selection], pause_marker_y[pause_selection], 100U, OBJ_PAL0);
		setSprite(pause_marker_x1[pause_selection]+8U, pause_marker_y[pause_selection], 102U, OBJ_PAL0);

		setSprite(pause_marker_x2[pause_selection], pause_marker_y[pause_selection], 104U, OBJ_PAL0);
		setSprite(pause_marker_x2[pause_selection]+8U, pause_marker_y[pause_selection], 106U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}
}
