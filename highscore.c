#include <gb/gb.h>
#include "defines.h"
#include "binconst.h"
#include "gamestate.h"
#include "fade.h"
#include "cos.h"
#include "highscore.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"

#include "data/bg/circles.h"
#include "data/bg/highscore.h"

UBYTE highscore_circle_index;
UBYTE highscore_selection;

void initHighscore() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, characters_data_length, characters_data);
	set_bkg_data(37U, circles_data_length, circles_data);
	set_bkg_data(highscore_offset, highscore_data_length, highscore_data);

	set_bkg_tiles(0U, 0U, highscore_tiles_width, highscore_tiles_height, highscore_tiles);

	set_sprite_data(0U, arrow_data_length, arrow_data);

	highscore_circle_index = 0U;;
	highscore_selection = level;

	OBP0_REG = B8(11010000);
	BGP_REG = B8(11100100);

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
	set_bkg_data(72U, 1U, &circles_data[(highscore_circle_index << 4)]);
}

void highscoreUpdateScreen() {
	fadeToWhite(4U);
	DISPLAY_OFF;
	_highscoreUpdateScreen();
	DISPLAY_ON;
	fadeFromWhite(4U);
}

void _highscoreUpdateScreen() {
	UBYTE x, y;
	UBYTE tile;
	UBYTE *data = level_names[highscore_selection];
	// Set name
	for(x = 7U; x != 13U; ++x) {
		set_bkg_tiles(x, 6U, 1U, 1U, data);
		data++;
	}

	tile = 1U;
	for(y = 8U; y != 13U; ++y) {
		set_bkg_tiles(3U, y, 1U, 1U, &tile);
		tile++;
	}
	tile = 22U; // 'L'
	set_bkg_tiles(3U, 14U, 1U, 1U, &tile);
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

		setSprite(12U-offset, 96U, 0U, OBJ_PAL0);
		setSprite(20U-offset, 96U, 1U, OBJ_PAL0);
		setSprite(12U-offset, 104U, 2U, OBJ_PAL0);
		setSprite(20U-offset, 104U, 3U, OBJ_PAL0);

		setSprite(148U+offset, 96U, 1U, OBJ_PAL0 | FLIP_X);
		setSprite(156U+offset, 96U, 0U, OBJ_PAL0 | FLIP_X);
		setSprite(148U+offset, 104U, 3U, OBJ_PAL0 | FLIP_X);
		setSprite(156U+offset, 104U, 2U, OBJ_PAL0 | FLIP_X);

		clearRemainingSprites();
		wait_vbl_done();
	}

	fadeToWhite(10U);
}
