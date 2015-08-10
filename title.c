#include <gb/gb.h>
#include <rand.h>
#include "defines.h"
#include "title.h"
#include "fade.h"
#include "gamestate.h"

#include "data/bg/titlescreen.h"
#include "data/sprite/characters.h"

extern UBYTE title_song_data;

const UBYTE title_message[11] = {
	26U, 28U, 15U, 29U,
	29U, 10U, 29U, 30U,
	11U, 28U, 30U
};

const UBYTE konami_code[10] = {
	J_UP,
	J_UP,
	J_DOWN,
	J_DOWN,
	J_LEFT,
	J_RIGHT,
	J_LEFT,
	J_RIGHT,
	J_B,
	J_A
};

UBYTE cheat_input[16];
UBYTE cheat_offset;

void initTitle() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data_rle(0U, titlescreen_data_length, titlescreen_data);
	set_bkg_tiles_rle(0U, 0U, titlescreen_tiles_width, titlescreen_tiles_height, titlescreen_tiles);
	set_sprite_data(0U, 37U, characters_data);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	clearSprites();

	setMusicBank(4U);
	playMusic(&title_song_data);

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void checkCheats() {
	UBYTE i, j;
	// Check Konami code
	i = 0U;
	j = (cheat_offset + 6U) & 15U;
	for(; i != 10U; ++i) {
		if(konami_code[i] != cheat_input[j]) return;
		j = (j + 1) & 15U;
	}

	levels_completed = 3U;
}

void enterTitle() {
	UBYTE i;

	initTitle();

	fadeFromWhite(10U);

	selection = level = 1U;
	cheat_offset = 0U;

	ticks = 0U;
	while(1) {
		updateJoystate();
		if(CLICKED(J_UP)) cheat_input[cheat_offset++] = J_UP;
		else if(CLICKED(J_DOWN)) cheat_input[cheat_offset++] = J_DOWN;
		else if(CLICKED(J_LEFT)) cheat_input[cheat_offset++] = J_LEFT;
		else if(CLICKED(J_RIGHT)) cheat_input[cheat_offset++] = J_RIGHT;
		else if(CLICKED(J_A)) cheat_input[cheat_offset++] = J_A;
		else if(CLICKED(J_B)) cheat_input[cheat_offset++] = J_B;
		cheat_offset = cheat_offset & 15U;

		if(CLICKED(J_START)) {
			checkCheats();

			initrand(DIV_REG);
			gamestate = GAMESTATE_SELECT;
			break;
		}

		if(ticks < 80U) {
			for(i = 0U; i != 11U; ++i) {
				if(i != 5U) {
					setSprite(48U + (i << 3), 136U, title_message[i], OBJ_PAL0);
				}
			}
		}
		ticks++;
		if(ticks == 100U) ticks = 0U;

		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(10U);
}
