#include <gb/gb.h>
#include "binconst.h"
#include "gamestate.h"
#include "fade.h"
#include "highscore.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"

#include "data/bg/circles.h"
#include "data/bg/highscore.h"

UBYTE highscore_circle_index;

void initHighscore() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, characters_data_length, characters_data);
	set_bkg_data(37U, circles_data_length, circles_data);
	set_bkg_data(highscore_offset, highscore_data_length, highscore_data);

	set_bkg_tiles(0U, 0U, highscore_tiles_width, highscore_tiles_height, highscore_tiles);

	highscore_circle_index;
	BGP_REG = B8(11100100);

	HIDE_SPRITES;
	HIDE_WIN;
	SHOW_BKG;
	DISPLAY_ON;
	enable_interrupts();
}

void highscoreScrollCircles() {
	highscore_circle_index = (highscore_circle_index+1U) & 7U;
	set_bkg_data(72U, 1U, &circles_data[(highscore_circle_index << 4)]);
}

void enterHighscore() {
	initHighscore();

	fadeFromWhite(10U);

	while(1) {
		ticks++;
		if((ticks & 3U) == 3U) {
			highscoreScrollCircles();
		}

		updateJoystate();
		if(CLICKED(J_START) || CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
			break;
		}
		wait_vbl_done();
	}

	fadeToWhite(10U);
}
