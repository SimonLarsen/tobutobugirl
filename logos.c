#include <gb/gb.h>
#include <gb/sample.h>
#include "defines.h"
#include "gamestate.h"
#include "logos.h"
#include "fade.h"
#include "sound.h"

#include "data/bg/tangram.h"
#include "data/bg/potato.h"
#include "data/sprite/shine.h"
#include "data/samples/tangram.h"

extern UBYTE tangram_shine_data;
extern UBYTE potato_jingle_data;

void initLogos() {
	disable_interrupts();
	DISPLAY_OFF;

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	set_bkg_data_rle(0U, tangram_data_length, tangram_data);
	set_bkg_tiles_rle(0U, 0U, tangram_tiles_width, tangram_tiles_height, tangram_tiles);

	set_sprite_data(0U, shine_data_length, shine_data);

	DISPLAY_ON;
	enable_interrupts();
}

void enterLogos() {
	UBYTE i;

	initLogos();

	fadeFromWhite(8U);

	for(i = 0U; i != 50U; ++i) wait_vbl_done();

	for(i = 10U; i != 142U; i += 6U) {
		if(i < 64U) {
			setSprite(i+8U, 80U, 0U, OBJ_PAL0);
			setSprite(i+16U, 80U, 2U, OBJ_PAL0);
			setSprite(i+24U, 80U, 4U, OBJ_PAL0);
		} else {
			setSprite(i+4U, 88U, 0U, OBJ_PAL0);
			setSprite(i+12U, 88U, 2U, OBJ_PAL0);
			setSprite(i+20U, 88U, 4U, OBJ_PAL0);
		}

		setSprite(i, 96U, 0U, OBJ_PAL0);
		setSprite(i+8U, 96U, 2U, OBJ_PAL0);
		setSprite(i+16U, 96U, 4U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	clearRemainingSprites();

	disable_interrupts();
	setMusicBank(9U);
	playMusic(&tangram_shine_data);
	enable_interrupts();

	for(i = 0U; i != 30U; ++i) {
		snd_update();
		wait_vbl_done();
	}

	stopMusic();
	play_sample(tangram_sample_data, tangram_sample_samples);

	for(i = 0U; i != 60U; ++i) wait_vbl_done();
	
	fadeToWhite(8U);

	disable_interrupts();
	DISPLAY_OFF;
	set_bkg_data_rle(0U, potato_data_length, potato_data);
	set_bkg_tiles_rle(0U, 0U, potato_tiles_width, potato_tiles_height, potato_tiles);

	DISPLAY_ON;
	enable_interrupts();

	fadeFromWhite(8U);

	disable_interrupts();

	for(i = 0U; i != 10U; ++i) wait_vbl_done();

	disable_interrupts();
	setMusicBank(9U);
	playMusic(&potato_jingle_data);
	enable_interrupts();
	enable_interrupts();

	for(i = 0U; i != 160U; ++i) wait_vbl_done();

	stopMusic();
	fadeToWhite(8U);

	gamestate = GAMESTATE_INTRO;
}
