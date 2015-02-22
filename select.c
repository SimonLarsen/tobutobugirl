#include <gb/gb.h>
#include "select.h"
#include "fade.h"
#include "gamestate.h"

#include "data/bg/circles.h"
#include "data/bg/select.h"

UBYTE selection;
UBYTE select_circle_index;

void initSelect() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, circles_data_length, circles_data);
	set_bkg_data(circles_data_length, select_data_length, select_data);
	set_bkg_tiles(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

	select_circle_index = 0U;
	ticks = 0U;

	HIDE_SPRITES;
	HIDE_WIN;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterSelect() {
	initSelect();

	fadeFromWhite(10U);

	while(1) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			select_circle_index = (select_circle_index+1U) & 7U;
			set_bkg_data(33U, 1U, &circles_data[(select_circle_index << 4)]);
		}

		if(CLICKED(J_START)) {
			break;
		}

		wait_vbl_done();
	}

	fadeToWhite(10U);
}
