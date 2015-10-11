#include <gb/gb.h>
#include "ending.h"
#include "gamestate.h"
#include "fade.h"
#include "data/bg/intro_bg.h"

void initEnding() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11101000

	set_bkg_data_rle(0U, intro_bg_data_length, intro_bg_data);
	set_bkg_tiles_rle(0U, 0U, intro_bg_tiles_width, intro_bg_tiles_height, intro_bg_tiles);

	clearSprites();

	HIDE_WIN;
	SHOW_BKG;
	SHOW_SPRITES;
	SPRITES_8x16;

	DISPLAY_ON;
	enable_interrupts();
}

void enterEnding() {
	gamestate = GAMESTATE_UNLOCKED;
}
