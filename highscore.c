#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "cos.h"
#include "highscore.h"
#include "ram.h"
#include "sound.h"

#include "data/sprite/characters.h"
#include "data/sprite/arrow.h"
#include "data/sprite/empty.h"

#include "data/bg/circles.h"
#include "data/bg/highscore.h"
#include "data/bg/selection1.h"
#include "data/bg/selection2.h"
#include "data/bg/selection3.h"
#include "data/bg/selection4.h"
#include "data/bg/selection_locked.h"

extern UBYTE highscore_song_data;
extern UBYTE dream_score_song_data;

void initHighscore() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, 38U, characters_data);
	set_bkg_data(38U, circles_data_length, circles_data);
	set_bkg_data(highscore_tiles_offset, highscore_data_length, highscore_data);

	set_bkg_tiles(0U, 0U, highscore_tiles_width, highscore_tiles_height, highscore_tiles);
	set_sprite_data(0U, arrow_data_length, arrow_data);
	set_sprite_data(arrow_data_length, 1U, empty_data);

	ticks = 0U;
	circle_index = 0U;
	sub_selection = level;

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
	tile = sub_selection;
	if(tile > levels_completed+1U) {
		tile = 0U;
	}

	switch(tile) {
		case 1U:
			set_bkg_data(selection1_tiles_offset, selection1_data_length, selection1_data);
			data = selection1_tiles;
			break;
		case 2U:
			set_bkg_data(selection2_tiles_offset, selection2_data_length, selection2_data);
			data = selection2_tiles;
			break;
		case 3U:
			set_bkg_data(selection3_tiles_offset, selection3_data_length, selection3_data);
			data = selection3_tiles;
			break;
		case 4U:
			set_bkg_data(selection4_tiles_offset, selection4_data_length, selection4_data);
			data = selection4_tiles;
			break;
		case 0U:
			set_bkg_data(selection_locked_tiles_offset, selection_locked_data_length, selection_locked_data);
			data = selection_locked_tiles;
			break;
	}

	set_bkg_tiles(0U, 4U, 20U, 6U, data);
	set_bkg_tiles(5U, 8U, 10U, 1U, highscore_tiles+165U);
	set_bkg_tiles(5U, 9U, 10U, 1U, highscore_tiles+185U);

	// Set level name
	data = level_names[tile];
	set_bkg_tiles(7U, 9U, 6U, 1U, data);

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	// Set scores
	data = &ram_data[(sub_selection-1U) << 4];
	for(i = 0U; i != 5U; ++i) {
		tile = 10U;
		for(j = 3U; j != 8U; ++j) {
			set_bkg_tiles(j, i+11U, 1U, 1U, &tile);
		}
		for(j = 10U; j != 14U; ++j) {
			set_bkg_tiles(j, i+11U, 1U, 1U, &tile);
		}
		set_bkg_tiles(16U, i+11U, 1U, 1U, &tile);

		if(data[0] != 0U) {
			// Draw time
			tile = data[0] / 60U;
			set_bkg_tiles(10U, i+11U, 1U, 1U, &tile);

			tile = 37U;
			set_bkg_tiles(11U, i+11U, 1U, 1U, &tile);

			tile = (data[0] % 60U) / 10U;
			set_bkg_tiles(12U, i+11U, 1U, 1U, &tile);

			tile = (data[0] % 60U) % 10U;
			set_bkg_tiles(13U, i+11U, 1U, 1U, &tile);

			// Draw score
			if(data[1] >= 100U) {
				tile = data[1] / 100U;
				set_bkg_tiles(3U, i+11U, 1U, 1U, &tile);
			}

			if(data[1] >= 10U) {
				tile = (data[1] / 10U) % 10U;
				set_bkg_tiles(4U, i+11U, 1U, 1U, &tile);
			}

			tile = data[1] % 10U;
			set_bkg_tiles(5U, i+11U, 1U, 1U, &tile);

			tile = 0U;
			set_bkg_tiles(6U, i+11U, 1U, 1U, &tile);
			set_bkg_tiles(7U, i+11U, 1U, 1U, &tile);

			// Draw rank
			tile = rank_letters[getRank(data[1], sub_selection)];
			set_bkg_tiles(16U, i+11U, 1U, 1U, &tile);
		}
		data += 2U;
	}

	DISABLE_RAM_MBC1;
}

void enterHighscore() {
	UBYTE offset, j;

	initHighscore();

	fadeFromWhite(6U);

	disable_interrupts();
	if(ending_flags & ENDING_FLAG_FROM_DREAM) {
		setMusicBank(6U);
		playMusic(&dream_score_song_data);
	} else {
		setMusicBank(4U);
		playMusic(&highscore_song_data);
	}
	ending_flags = 0U;
	enable_interrupts();

	while(1) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			circle_index = (circle_index+1U) & 7U;
			set_bkg_data(47U, 1U, &circles_data[(circle_index << 4)]);
		}

		if(ISDOWN(J_LEFT)) {
			sub_selection--;
			if(sub_selection == 0U) sub_selection = 4U;
			playSound(SFX_HIGHSCORE_SWITCH);
			highscoreUpdateScreen();
		}
		if(ISDOWN(J_RIGHT)) {
			sub_selection++;
			if(sub_selection == 5U) sub_selection = 1U;
			playSound(SFX_HIGHSCORE_SWITCH);
			highscoreUpdateScreen();
		}

		if(CLICKED(J_START) || CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
			playSound(SFX_MENU_CANCEL);
			break;
		}

		offset = cos32_64[(ticks & 63U)] >> 3;

		// Draw arrows
		setSprite(20U-offset, 64U, 0U, OBJ_PAL0);
		setSprite(28U-offset, 64U, 2U, OBJ_PAL0);
		setSprite(20U-offset, 72U, 1U, OBJ_PAL0);
		setSprite(28U-offset, 72U, 3U, OBJ_PAL0);

		setSprite(140U+offset, 64U, 2U, OBJ_PAL0 | FLIP_X);
		setSprite(148U+offset, 64U, 0U, OBJ_PAL0 | FLIP_X);
		setSprite(140U+offset, 72U, 3U, OBJ_PAL0 | FLIP_X);
		setSprite(148U+offset, 72U, 1U, OBJ_PAL0 | FLIP_X);

		if((ticks & 63U) < 16U
		&& last_highscore_level == sub_selection
		&& last_highscore_slot < 5U) {
			offset = 104U + (last_highscore_slot << 3);

			for(j = 4U; j != 9U; ++j) {
				setSprite(j<<3, offset, 4U, OBJ_PAL0);
			}
			for(j = 11U; j != 15U; ++j) {
				setSprite(j<<3, offset, 4U, OBJ_PAL0);
			}
			setSprite(136U, offset, 4U, OBJ_PAL0);
		}

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	// Clear blinking after exit
	last_highscore_slot = 5U;

	stopMusic();
	clearRemainingSprites();
	fadeToWhite(6U);
	wait_sound_done();
}
