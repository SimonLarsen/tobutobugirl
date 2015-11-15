#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "ram.h"
#include "options.h"

#include "data/sprite/characters.h"
#include "data/bg/options.h"
#include "data/sprite/select_arrow.h"
#include "data/sprite/skins.h"

UBYTE options_selection;

const UBYTE on_str[] = {10U, 25U, 24U};
const UBYTE off_str[] = {25U, 16U, 16U};

extern UBYTE highscore_song_data;

void initOptions() {
	disable_interrupts();
	DISPLAY_OFF;

	BGP_REG = 0xE4U;  // 11100100
	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xFFU; // 11111111

	move_bkg(0U, 0U);

	set_bkg_data(0U, 40U, characters_data);
	set_bkg_data_rle(options_offset, options_data_length, options_data);
	set_bkg_tiles_rle(0U, 0U, options_tiles_width, options_tiles_height, options_tiles);

	set_sprite_data(0U, select_arrow_data_length, select_arrow_data);
	set_sprite_data(4U, skins_data_length, skins_data);

	options_selection = 0U;

	// Draw ON/OFF
	if(options_show_dash) set_bkg_tiles(15U, 5U, 3U, 1U, on_str);
	else set_bkg_tiles(15U, 5U, 3U, 1U, off_str);

	if(options_reverse_keys) set_bkg_tiles(15U, 7U, 3U, 1U, on_str);
	else set_bkg_tiles(15U, 7U, 3U, 1U, off_str);

	setMusicBank(4U);
	playMusic(&highscore_song_data);

	SHOW_BKG;
	SHOW_SPRITES;
	HIDE_WIN;
	SPRITES_8x16;

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
	SWITCH_RAM_MBC1(0);
	ram_data[RAM_SHOW_DASH] ^= 1U;
	DISABLE_RAM_MBC1;
}

void toggleReverseKeys() {
	options_reverse_keys ^= 1U;
	disable_interrupts();
	if(options_reverse_keys) set_bkg_tiles(15U, 7U, 3U, 1U, on_str);
	else set_bkg_tiles(15U, 7U, 3U, 1U, off_str);
	enable_interrupts();

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);
	ram_data[RAM_REVERSE_KEYS] ^= 1U;
	DISABLE_RAM_MBC1;
}

void enterOptions() {
	UBYTE i, x, y, pal;
	initOptions();

	fadeFromWhite(6U);

	while(gamestate == GAMESTATE_OPTIONS) {
		updateJoystate();
		ticks++;

		if(CLICKED(J_A)) {
			if(options_selection == 0U) toggleShowDash();
			else if(options_selection == 1U) toggleReverseKeys();
			else if(options_selection == 2U) {
				ENABLE_RAM_MBC1;
				SWITCH_RAM_MBC1(0);
				options_player_skin++;
				ram_data[RAM_PLAYER_SKIN]++;
				if(options_player_skin > levels_completed+1U) {
					options_player_skin = 1U;
					ram_data[RAM_PLAYER_SKIN] = 1U;
				}
				DISABLE_RAM_MBC1;
			}
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

		// Draw skins
		x = 24U;
		pal = OBJ_PAL0;
		for(i = 1U; i != 6U; ++i) {
			if(i > levels_completed+1U) pal = OBJ_PAL1;

			y = 128U;

			if(i == options_player_skin) {
				setSprite(x+4U, 144U, 2U, OBJ_PAL0);
				y -= 2U;
			}

			setSprite(x, y, (i << 2), pal);
			setSprite(x+8U, y, (i << 2)+2U, pal);

			x += 28U;
		}

		clearRemainingSprites();
		wait_vbl_done();
	}
	
	clearRemainingSprites();
	fadeToWhite(6U);
}
