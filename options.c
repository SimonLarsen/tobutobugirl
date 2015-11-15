#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "ram.h"
#include "options.h"

#include "data/sprite/characters.h"
#include "data/bg/options.h"
#include "data/sprite/select_arrow.h"

UBYTE options_selection;

const UBYTE on_str[] = {10U, 25U, 24U};
const UBYTE off_str[] = {25U, 16U, 16U};

void initOptions() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data(0U, 40U, characters_data);
	set_bkg_data_rle(options_offset, options_data_length, options_data);
	set_bkg_tiles_rle(0U, 0U, options_tiles_width, options_tiles_height, options_tiles);

	set_sprite_data(0U, select_arrow_data_length, select_arrow_data);

	options_selection = 0U;

	// Draw ON/OFF
	options_show_dash ^= 1U;
	options_reverse_keys ^= 1U;
	toggleShowDash();
	toggleReverseKeys();

	SHOW_BKG;
	SHOW_SPRITES;
	HIDE_WIN;

	DISPLAY_ON;
	enable_interrupts();
}

void toggleShowDash() {
	options_show_dash ^= 1U;
	disable_interrupts();
	if(options_show_dash) set_bkg_tiles(15U, 5U, 3U, 1U, on_str);
	else set_bkg_tiles(15U, 5U, 3U, 1U, off_str);
	enable_interrupts();

	ENABLE_RAM_MBC1;
	ram_data[RAM_SHOW_DASH] = options_show_dash;
	DISABLE_RAM_MBC1;
}

void toggleReverseKeys() {
	options_reverse_keys ^= 1U;
	disable_interrupts();
	if(options_reverse_keys) set_bkg_tiles(15U, 7U, 3U, 1U, on_str);
	else set_bkg_tiles(15U, 7U, 3U, 1U, off_str);
	enable_interrupts();

	ENABLE_RAM_MBC1;
	ram_data[RAM_REVERSE_KEYS] = options_reverse_keys;
	DISABLE_RAM_MBC1;
}

void enterOptions() {
	initOptions();

	fadeFromWhite(6U);

	while(gamestate == GAMESTATE_OPTIONS) {
		updateJoystate();
		ticks++;

		if(CLICKED(J_A)) {
			if(options_selection == 0U) toggleShowDash();
			else if(options_selection == 1U) toggleReverseKeys();
		}

		if(CLICKED(J_UP)) {
			if(options_selection) options_selection--;
			else options_selection = 2U;
		}
		if(CLICKED(J_DOWN)) {
			options_selection++;
			if(options_selection == 3U) options_selection = 0U;
		}

		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
		}

		// Update sprites
		if(options_selection <= 1U) {
			setSprite(16U, 56U+(options_selection << 4), 0U, OBJ_PAL0);
		}
		else if(options_selection == 2U) {
			setSprite(16U, 112U, 0U, OBJ_PAL0);
		}

		clearRemainingSprites();
		wait_vbl_done();
	}
	
	clearRemainingSprites();
	fadeToWhite(6U);
}
