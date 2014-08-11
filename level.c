#include <gb/gb.h>
#include "defines.h"
#include "binconst.h"
#include "fade.h"
#include "gamestate.h"
#include "input.h"

#include "level.h"

#include "data/bg/level.h"
#include "data/bg/leveltext.h"
#include "data/sprite/numbers.h"
#include "data/sprite/levelsprites.h"

const UBYTE yoffsets[4] = {0U, 1U, 2U, 1U};

#define SPR_GIRL	0U
#define SPR_NUMBER1 8U
#define SPR_NUMBER2 9U
#define SPR_COMPL1  10U
#define SPR_COMPL2  11U
#define SPR_COMPL3  12U
#define SPR_COMPL4  13U

UBYTE time, frame, y;

void updateLevelScreen() {
	set_sprite_tile(SPR_NUMBER1, (level+1U) / 10);
	set_sprite_tile(SPR_NUMBER2, (level+1U) % 10);

	if(completed[level]) {
		move_sprite(SPR_COMPL1, 80U, 120U);
		move_sprite(SPR_COMPL2, 88U, 120U);
		move_sprite(SPR_COMPL3, 80U, 128U);
		move_sprite(SPR_COMPL4, 88U, 128U);
	} else {
		move_sprite(SPR_COMPL1, 0U, 120U);
		move_sprite(SPR_COMPL2, 0U, 120U);
		move_sprite(SPR_COMPL3, 0U, 120U);
		move_sprite(SPR_COMPL4, 0U, 120U);
	}
}

void enterLevel() {
	UBYTE tmp, i, ix, iy, loop;

	disable_interrupts();
	DISPLAY_OFF;

	set_bkg_data(0U, level_data_length, level_data);
	set_win_data(level_data_length, leveltext_data_length, leveltext_data);
	set_bkg_tiles(0U, 0U, level_tiles_width, level_tiles_height, level_tiles);

	set_sprite_data(0U, numbers_data_length, numbers_data);
	set_sprite_data(numbers_data_length, levelsprites_data_length, levelsprites_data);

	SPRITES_8x8;

	OBP0_REG = B8(00000000);
	OBP1_REG = B8(10010000);

	// Clear win tiles
	tmp = 0U;
	for(iy = 0; iy < 6U; ++iy) {
		for(ix = 0U; ix < 20U; ++ix) {
			set_win_tiles(ix, iy, 1U, 1U, &tmp);
		}
	}

	set_win_tiles(4U, 0U, 12U, 1U, leveltext_tiles);
	move_bkg(7U, 0U);
	move_win(7U, 96U);

	clearSprites();

	for(i = 0U; i < 10U; ++i) set_sprite_prop(i, B8(00010000));
	for(i = 10U; i <= 14U; ++i) set_sprite_prop(i, B8(00010000));

	set_sprite_prop(SPR_NUMBER1, 0U);
	set_sprite_prop(SPR_NUMBER2, 0U);
	move_sprite(SPR_NUMBER1, 104U, 111U);
	move_sprite(SPR_NUMBER2, 112U, 111U);

	set_sprite_tile(SPR_COMPL1, 26U);
	set_sprite_tile(SPR_COMPL2, 27U);
	set_sprite_tile(SPR_COMPL3, 28U);
	set_sprite_tile(SPR_COMPL4, 29U);

	updateLevelScreen();

	SHOW_BKG;
	SHOW_SPRITES;
	SHOW_WIN;

	DISPLAY_ON;
	enable_interrupts();

	fadeFromWhite();

	frame = 0U;
	time = 0U;
	y = 36U;
	loop = 1U;
	while(loop) {
		if((time & 15U) == 15U) frame = frame ^ 8U;
		y = yoffsets[(time >> 4U) & 3U];

		i = 0U;
		for(iy = 0U; iy < 4U; ++iy) {
			for(ix = 0U; ix < 2U; ++ix) {
				move_sprite(i, 24U+(ix << 3U), 36U+(iy << 3U)+y);

				set_sprite_tile(i, 10U+frame+i);
				++i;
			}
		}

		updateJoystate();

		if(CLICKED(J_LEFT)) {
			if(level > 0) {
				level--;
				updateLevelScreen();
			}
		} else if(CLICKED(J_RIGHT)) {
			if(level < NUM_LEVELS-1U) {
				level++;
				updateLevelScreen();
			}
		} else if(CLICKED(J_START)) {
			gamestate = GAMESTATE_GAME;
			loop = 0U;
		}

		time++;
		wait_vbl_done();
	}

	fadeToWhite();
}
