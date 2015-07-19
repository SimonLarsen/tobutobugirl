#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "cos.h"
#include "highscore.h"
#include "ram.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"
#include "data/sprite/empty.h"

#include "data/bg/circles.h"
#include "data/bg/highscore.h"
#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"

UBYTE highscore_circle_index;
UBYTE highscore_selection;

void initHighscore() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, 38U, characters_data);
	set_bkg_data(38U, circles_data_length, circles_data);
	set_bkg_data(highscore_offset, highscore_data_length, highscore_data);

	set_bkg_tiles(0U, 0U, highscore_tiles_width, highscore_tiles_height, highscore_tiles);
	set_sprite_data(0U, arrow_data_length, arrow_data);
	set_sprite_data(arrow_data_length, 1U, empty_data);

	highscore_circle_index = 0U;;
	highscore_selection = level;

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	clearSprites();
	_highscoreUpdateScreen();

	SPRITES_8x8;
	SHOW_SPRITES;
	HIDE_WIN;
	SHOW_BKG;
	DISPLAY_ON;
	enable_interrupts();
}

void highscoreScrollCircles() {
	highscore_circle_index = (highscore_circle_index+1U) & 7U;
	set_bkg_data(142U, 1U, &circles_data[(highscore_circle_index << 4)]);
}

void highscoreUpdateScreen() {
	clearRemainingSprites();
	fadeToWhite(4U);
	DISPLAY_OFF;

	_highscoreUpdateScreen();

	DISPLAY_ON;
	fadeFromWhite(4U);
}

void _highscoreUpdateScreen() {
	UBYTE i, j, tile;
	UBYTE *data;

	// Select level images
	if(highscore_selection == 1U) {
		set_bkg_data(selection1_offset, selection1_data_length, selection1_data);
		data = selection1_tiles;
	} else if(highscore_selection == 2U) {
		set_bkg_data(selection2_offset, selection2_data_length, selection2_data);
		data = selection2_tiles;
	} else if(highscore_selection == 3U) {
		set_bkg_data(selection3_offset, selection3_data_length, selection3_data);
		data = selection3_tiles;
	}
	set_bkg_tiles(0U, 4U, 20U, 6U, data);
	set_bkg_tiles(5U, 8U, 10U, 1U, highscore_tiles+165U);
	set_bkg_tiles(5U, 9U, 10U, 1U, highscore_tiles+185U);

	// Set level name
	data = level_names[highscore_selection];
	set_bkg_tiles(7U, 9U, 6U, 1U, data);

	// Set numbers
	for(i = 0U; i != 5U; ++i) {
		tile = i+1U;
		set_bkg_tiles(3U, i+11U, 1U, 1U, &tile);
	}

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	// Set scores
	data = &ram_data[(highscore_selection-1U) << 4];
	for(i = 0U; i != 5U; ++i) {
		tile = 10U;
		for(j = 6U; j != 11U; ++j) {
			set_bkg_tiles(j, i+11U, 1U, 1U, &tile);
		}
		for(j = 13U; j != 17U; ++j) {
			set_bkg_tiles(j, i+11U, 1U, 1U, &tile);
		}

		if(data[0] != 0U) {
			// Draw time
			tile = data[0] / 60U;
			set_bkg_tiles(13U, i+11U, 1U, 1U, &tile);

			tile = 37U;
			set_bkg_tiles(14U, i+11U, 1U, 1U, &tile);

			tile = (data[0] % 60U) / 10U;
			set_bkg_tiles(15U, i+11U, 1U, 1U, &tile);

			tile = (data[0] % 60U) % 10U;
			set_bkg_tiles(16U, i+11U, 1U, 1U, &tile);

			// Draw score
			if(data[1] >= 100U) {
				tile = data[1] / 100U;
				set_bkg_tiles(6U, i+11U, 1U, 1U, &tile);
			}

			if(data[1] >= 10U) {
				tile = (data[1] / 10U) % 10U;
				set_bkg_tiles(7U, i+11U, 1U, 1U, &tile);
			}

			tile = data[1] % 10U;
			set_bkg_tiles(8U, i+11U, 1U, 1U, &tile);

			tile = 0U;
			set_bkg_tiles(9U, i+11U, 1U, 1U, &tile);
			set_bkg_tiles(10U, i+11U, 1U, 1U, &tile);
		}
		data += 2U;
	}

	DISABLE_RAM_MBC1;
}

void enterHighscore() {
	UBYTE offset;

	initHighscore();

	fadeFromWhite(10U);

	while(1) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			highscoreScrollCircles();
		}

		if(CLICKED(J_LEFT)) {
			highscore_selection--;
			if(highscore_selection == 0U) highscore_selection = 3U;
			highscoreUpdateScreen();
		}
		if(CLICKED(J_RIGHT)) {
			highscore_selection++;
			if(highscore_selection == 4U) highscore_selection = 1U;
			highscoreUpdateScreen();
		}

		if(CLICKED(J_START) || CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
			break;
		}

		offset = cos32_64[(ticks & 63U)] >> 3;

		// Draw arrows
		setSprite(20U-offset, 64U, 0U, OBJ_PAL0);
		setSprite(28U-offset, 64U, 1U, OBJ_PAL0);
		setSprite(20U-offset, 72U, 2U, OBJ_PAL0);
		setSprite(28U-offset, 72U, 3U, OBJ_PAL0);

		setSprite(140U+offset, 64U, 1U, OBJ_PAL0 | FLIP_X);
		setSprite(148U+offset, 64U, 0U, OBJ_PAL0 | FLIP_X);
		setSprite(140U+offset, 72U, 3U, OBJ_PAL0 | FLIP_X);
		setSprite(148U+offset, 72U, 2U, OBJ_PAL0 | FLIP_X);

		if((ticks & 63U) < 16U
		&& last_highscore_level == highscore_selection
		&& last_highscore_slot < 5U) {
			offset = 104U + (last_highscore_slot << 3);
			setSprite(32U, offset, 4U, OBJ_PAL0);

			setSprite(56U, offset, 4U, OBJ_PAL0);
			setSprite(64U, offset, 4U, OBJ_PAL0);
			setSprite(72U, offset, 4U, OBJ_PAL0);
			setSprite(80U, offset, 4U, OBJ_PAL0);
			setSprite(88U, offset, 4U, OBJ_PAL0);

			setSprite(112U, offset, 4U, OBJ_PAL0);
			setSprite(120U, offset, 4U, OBJ_PAL0);
			setSprite(128U, offset, 4U, OBJ_PAL0);
			setSprite(136U, offset, 4U, OBJ_PAL0);
		}

		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(10U);
}
