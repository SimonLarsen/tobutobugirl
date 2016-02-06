#include <gb/gb.h>
#include <gb/sample.h>
#include "logos.h"
#include "fade.h"
#include "gamestate.h"

#include "data/bg/tangram.h"
#include "data/bg/potato.h"
#include "data/samples/tangram.h"

#define LOGO_DELAY 2U

extern UBYTE potato_jingle_data;

void initLogos() {
	disable_interrupts();
	DISPLAY_OFF;

	BGP_REG = 0xE4U; // 11100100

	HIDE_WIN;
	HIDE_SPRITES;
	SHOW_BKG;

	set_bkg_data_rle(0U, tangram_data_length, tangram_data);
	set_bkg_tiles_rle(0U, 0U, tangram_tiles_width, tangram_tiles_height, tangram_tiles);

	DISPLAY_ON;
	enable_interrupts();
}

void enterLogos() {
	UBYTE i;

	initLogos();

	fadeFromWhite(8U);

	ticks = 0U;
	i = LOGO_DELAY;
	while(i != 0U) {
		ticks++;

		if(ticks == 20U && i == LOGO_DELAY) {
			play_sample(tangram_sample_data, tangram_sample_samples);
		}

		if(ticks == 60U) {
			i--;
			ticks = 0U;
		}

		wait_vbl_done();
	}
	
	fadeToWhite(8U);

	disable_interrupts();
	DISPLAY_OFF;
	set_bkg_data_rle(0U, potato_data_length, potato_data);
	set_bkg_tiles_rle(0U, 0U, potato_tiles_width, potato_tiles_height, potato_tiles);

	DISPLAY_ON;
	enable_interrupts();

	fadeFromWhite(8U);

	disable_interrupts();
	setMusicBank(9U);
	playMusic(&potato_jingle_data);
	enable_interrupts();

	ticks = 0U;
	i = LOGO_DELAY;
	while(i != 0U) {
		ticks++;
		if(ticks == 80U) {
			i--;
			ticks = 0U;
		}

		wait_vbl_done();
	}

	stopMusic();
	fadeToWhite(8U);

	gamestate = GAMESTATE_INTRO;
}
