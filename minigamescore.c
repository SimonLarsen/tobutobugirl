#include <gb/gb.h>
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "sound.h"
#include "ram.h"

#include "data/bg/minigame_score_bg.h"
#include "characters.h"
#include "zoom_circles.h"

void initMinigamescore() {
	UBYTE tile;

	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data(0U, 10U, characters_data);
	set_bkg_data(10U, zoom_circles_data_length, zoom_circles_data);
	set_bkg_data_rle(18U, minigame_score_bg_data_length, minigame_score_bg_data);
	set_bkg_data(18U, 8U, zoom_circles_data);

	set_bkg_tiles_rle(0U, 0U, minigame_score_bg_tiles_width, minigame_score_bg_tiles_height, minigame_score_bg_tiles);

	// draw this time
	if(elapsed_minutes >= 10U) {
		tile = elapsed_minutes / 10U;
		set_bkg_tiles(13U, 7U, 1U, 1U, &tile);
	}

	tile = elapsed_minutes % 10U;
	set_bkg_tiles(14U, 7U, 1U, 1U, &tile);

	tile = elapsed_time / 10U;
	set_bkg_tiles(16U, 7U, 1U, 1U, &tile);

	tile = elapsed_time % 10U;
	set_bkg_tiles(17U, 7U, 1U, 1U, &tile);

	// draw best time
	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0U);

	if(elapsed_minutes >= 10U) {
		tile = ram_data[RAM_MINIGAME_MIN] / 10U;
		set_bkg_tiles(13U, 10U, 1U, 1U, &tile);
	}

	tile = ram_data[RAM_MINIGAME_MIN] % 10U;
	set_bkg_tiles(14U, 10U, 1U, 1U, &tile);

	tile = ram_data[RAM_MINIGAME_SEC] / 10U;
	set_bkg_tiles(16U, 10U, 1U, 1U, &tile);

	tile = ram_data[RAM_MINIGAME_SEC] % 10U;
	set_bkg_tiles(17U, 10U, 1U, 1U, &tile);

	DISABLE_RAM_MBC1;

	ticks = 0U;
	circle_index = 0U;
	clearSprites();

	HIDE_WIN;
	HIDE_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterMinigamescore() {
	initMinigamescore();

	fadeFromWhite(6U);

	while(1U) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			circle_index = (circle_index+1U) & 7U;
			set_bkg_data(18U+circle_index, 8U-circle_index, zoom_circles_data);
			if(circle_index) {
				set_bkg_data(18U, circle_index, &zoom_circles_data[(8U-circle_index) << 4]);
			}
		}

		if(CLICKED(J_START) || CLICKED(J_A)) {
			break;
		}

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(6U);

	stopMusic();
	gamestate = GAMESTATE_TITLE;
}
