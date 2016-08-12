#include <gb/gb.h>
#include <string.h>
#include "gamestate.h"
#include "fade.h"
#include "winscreen.h"

#include "data/sprite/characters.h"
#include "data/bg/win_base.h"
#include "data/bg/win1.h"
#include "data/bg/win2.h"
#include "data/bg/win3.h"
#include "data/bg/win4.h"
#include "mmlgb/driver/music.h"
#include "mmlgb/driver/notes.h"
#include "mmlgb/driver/freq.h"

extern UBYTE score_tally_song_data;
extern UBYTE dream_score_song_data;

const UBYTE winscreen_clear_text[5] = {
	13U, 22U, 15U, 11U, 28U
};

const UBYTE sharkwave_data[16] = {
	1U, 35U, 69U, 103U, 138U, 166U, 205U, 239U, 255U, 134U, 67U, 50U, 162U, 17U, 16U, 0U
};

void drawScore(UBYTE x, UBYTE y, UBYTE value) {
	UBYTE tile;

	if(value >= 100U) {
		tile = value / 100U;
		set_bkg_tiles(x, y, 1U, 1U, &tile);
	}
	if(value >= 10U) {
		tile = (value / 10U) % 10U;
		set_bkg_tiles(x+1U, y, 1U, 1U, &tile);
	}
	if(value) {
		tile = value % 10U;
		set_bkg_tiles(x+2U, y, 1U, 1U, &tile);
		tile = 0U;
		set_bkg_tiles(x+3U, y, 1U, 1U, &tile);
	}
	tile = 0U;
	set_bkg_tiles(x+4U, y, 1U, 1U, &tile);
}

void winscreenPlayNote(UBYTE note, UBYTE octave) {
	UWORD freq3;

	freq3 = freq[(octave << 4) + note - MUS_FIRST_NOTE];

	NR30_REG = 0x0U;
	NR30_REG = 0x80U;
	NR32_REG = 0xFFU;
	NR33_REG = (UBYTE)freq3;
	NR34_REG = 0x80U | (freq3 >> 8);
}

void winscreenJingle() {
	delay(30U);
	winscreenPlayNote(T_Fs, 6U);
	delay(80U);
	winscreenPlayNote(T_A, 6U);
	delay(80U);
	winscreenPlayNote(T_E, 7U);
	delay(85U);
	NR30_REG = 0x0U;
}

void winscreenTextJingle() {
	delay(30U);
	winscreenPlayNote(T_E, 6U);
	delay(40U);
	winscreenPlayNote(T_B, 6U);
	delay(40U);
	winscreenPlayNote(T_A, 7U);
	delay(40U);
	winscreenPlayNote(T_E, 7U);
	delay(40U);
	winscreenPlayNote(T_Cs, 8U);
	delay(60U);
	NR30_REG = 0x0U;
}

void countUpScore(UBYTE x, UBYTE y, UBYTE value, UBYTE delay_time) {
	UBYTE i, j;

	j = 0U;
	for(i = 0U; i != value+1U; ++i) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			i = value;
		}

		switch(j) {
			case 0U: winscreenPlayNote(T_C, 6U); break;
			case 1U: winscreenPlayNote(T_D, 7U); break;
			case 2U: winscreenPlayNote(T_A, 6U); break;
		}

		j++;
		if(j == 3U) j = 0U;

		drawScore(x, y, i);
		delay(delay_time);
	}
}

void initWinscreen() {
	UBYTE *data;

	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	clearSprites();
	BGP_REG = 0xE4U; // 11100100

	set_bkg_data(0U, 40U, characters_data);
	set_bkg_data_rle(win_base_offset, win_base_data_length, win_base_data);
	set_bkg_tiles_rle(0U, 0U, win_base_tiles_width, win_base_tiles_height, win_base_tiles);
	set_bkg_tiles(11U, 1U, 5U, 1U, winscreen_clear_text);

	switch(level) {
		case 1:
			set_bkg_data_rle(win1_offset, win1_data_length, win1_data);
			set_bkg_tiles_rle(10U, 5U, win1_tiles_width, win1_tiles_height, win1_tiles);
			break;
		case 2:
			set_bkg_data_rle(win2_offset, win2_data_length, win2_data);
			set_bkg_tiles_rle(8U, 3U, win2_tiles_width, win2_tiles_height, win2_tiles);
			break;
		case 3:
			set_bkg_data_rle(win3_offset, win3_data_length, win3_data);
			set_bkg_tiles_rle(8U, 3U, win3_tiles_width, win3_tiles_height, win3_tiles);
			break;
		case 4:
			set_bkg_data_rle(win4_offset, win4_data_length, win4_data);
			set_bkg_tiles_rle(8U, 6U, win4_tiles_width, win4_tiles_height, win4_tiles);
			break;
	}

	// Set level name
	data = level_names[level];
	set_bkg_tiles(4U, 1U, 6U, 1U, data);

	// Set wave channel data
	NR30_REG = 0x0U;
	memcpy(0xFF30, sharkwave_data, 16U);

	HIDE_SPRITES;
	HIDE_WIN;
	SHOW_BKG;
	DISPLAY_ON;
	enable_interrupts();
}

void enterWinscreen() {
	UBYTE tile, tmp;
	initWinscreen();

	fadeFromWhite(10U);

	disable_interrupts();
	// Did we come from end of ending sequence?
	if(ending_finished) {
		mus_setPaused(0U);
	// Else did we complete Dream?
	} else if(level == 4U) {
		setMusicBank(6U);
		playMusic(&dream_score_song_data);
	// Else just play regular music
	} else {
		setMusicBank(4U);
		playMusic(&score_tally_song_data);
	}
	enable_interrupts();

	delay(255U);

	// Time
	tile = elapsed_time / 60U;
	set_bkg_tiles(1U, 5U, 1U, 1U, &tile);
	tile = 37U;
	set_bkg_tiles(2U, 5U, 1U, 1U, &tile);
	tile = (elapsed_time % 60U) / 10U;
	set_bkg_tiles(3U, 5U, 1U, 1U, &tile);
	tile = (elapsed_time % 60U) % 10U;
	set_bkg_tiles(4U, 5U, 1U, 1U, &tile);
	winscreenTextJingle();

	delay(512U);

	// Count up time bonus
	countUpScore(3, 6, TIME_BONUS, 30U);
	winscreenJingle();

	delay(512U);

	// Kills
	tmp = kills;
	tile = tmp / 10U;
	set_bkg_tiles(1U, 10U, 1U, 1U, &tile);
	tile = tmp % 10U;
	set_bkg_tiles(2U, 10U, 1U, 1U, &tile);
	winscreenTextJingle();

	delay(512U);

	// Count up kill bonus
	countUpScore(3U, 11U, KILL_BONUS, 30U);
	winscreenJingle();

	delay(512U);

	// Count up total score
	countUpScore(3U, 15U, TOTAL_SCORE, 30U);
	winscreenJingle();

	while(1) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			if(unlocked_bits) {
				gamestate = GAMESTATE_UNLOCKED;
			} else {
				gamestate = GAMESTATE_HIGHSCORE;
			}
			break;
		}
		wait_vbl_done();
	}

	fadeToWhite(10U);

	stopMusic();
}
