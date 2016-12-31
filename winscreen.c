#include <gb/gb.h>
#include <string.h>
#include <rand.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "winscreen.h"
#include "sound.h"

#include "data/sprite/characters.h"
#include "data/bg/win_base.h"
#include "data/bg/rank_banner.h"
#include "data/sprite/ranks.h"
#include "mmlgb/driver/music.h"
#include "mmlgb/driver/notes.h"
#include "mmlgb/driver/freq.h"

extern UBYTE score_tally_song_data;

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
	winscreenPlayNote(T_Fs, 6U); delay(80U);
	winscreenPlayNote(T_A,  6U); delay(80U);
	winscreenPlayNote(T_E,  7U); delay(85U);
	NR30_REG = 0x0U;
}

void winscreenTextJingle() {
	delay(30U);
	winscreenPlayNote(T_E,  6U); delay(40U);
	winscreenPlayNote(T_B,  6U); delay(40U);
	winscreenPlayNote(T_A,  7U); delay(40U);
	winscreenPlayNote(T_E,  7U); delay(40U);
	winscreenPlayNote(T_Cs, 8U); delay(60U);
	NR30_REG = 0x0U;
}

void winscreenShowRank() {
	UBYTE i, offset;

	disable_interrupts();
	set_bkg_tiles_rle(0U, 6U, 20U, 6U, rank_banner_tiles);
	enable_interrupts();

	delay(255U);

	for(offset = 64U; offset != 252U; offset -= 4U) {
		setSprite(104U-offset, 72U-offset, 0U, OBJ_PAL0);
		setSprite(112U-offset, 72U-offset, 2U, OBJ_PAL0);
		setSprite(120U+offset, 72U-offset, 4U, OBJ_PAL0);
		setSprite(128U+offset, 72U-offset, 6U, OBJ_PAL0);

		setSprite(104U-offset, 88U+offset,  8U, OBJ_PAL0);
		setSprite(112U-offset, 88U+offset, 10U, OBJ_PAL0);
		setSprite(120U+offset, 88U+offset, 12U, OBJ_PAL0);
		setSprite(128U+offset, 88U+offset, 14U, OBJ_PAL0);

		wait_vbl_done();
	}

	clearRemainingSprites();

	setSprite(104U, 72U, 0U, OBJ_PAL0);
	setSprite(112U, 72U, 2U, OBJ_PAL0);
	setSprite(120U, 72U, 4U, OBJ_PAL0);
	setSprite(128U, 72U, 6U, OBJ_PAL0);

	setSprite(104U, 88U, 8U, OBJ_PAL0);
	setSprite(112U, 88U, 10U, OBJ_PAL0);
	setSprite(120U, 88U, 12U, OBJ_PAL0);
	setSprite(128U, 88U, 14U, OBJ_PAL0);

	playSound(SFX_RANK_CRASH);

	// shake screen
	for(i = 0; i != 6U; ++i) {
		//move_bkg(((UBYTE)rand() & 4U) - 2U, ((UBYTE)rand() & 4U) - 2U);
		move_bkg(0U, ((UBYTE)rand() & 4U) - 2U);
		wait_vbl_done();
		snd_update();
		wait_vbl_done();
		snd_update();
	}

	move_bkg(0U, 0U);

	wait_sound_done();
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
	UBYTE rank;
	UBYTE *data;

	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	clearSprites();
	BGP_REG = 0xE4U; // 11100100

	set_bkg_data(0U, 40U, characters_data);
	set_bkg_data_rle(win_base_offset, win_base_data_length, win_base_data);
	set_bkg_data_rle(rank_banner_offset, rank_banner_data_length, rank_banner_data);

	set_bkg_tiles_rle(0U, 0U, win_base_tiles_width, win_base_tiles_height, win_base_tiles);
	set_bkg_tiles(11U, 1U, 5U, 1U, winscreen_clear_text);

	setWinscreenBackground(level);

	// Load rank sprite
	rank = getRank(TOTAL_SCORE, level);
	data = ranks_data;
	while(rank != 0U) {
		data += 256UL;
		--rank;
	}
	set_sprite_data(0U, 16U, data);

	// Set level name
	data = level_names[level];
	set_bkg_tiles(4U, 1U, 6U, 1U, data);

	// Set wave channel data
	NR30_REG = 0x0U;
	memcpy(0xFF30, sharkwave_data, 16U);

	HIDE_WIN;
	SHOW_BKG;
	SHOW_SPRITES;
	SPRITES_8x16;

	OBP0_REG = 0xD0U; // 11010000

	clearSprites();

	DISPLAY_ON;
	enable_interrupts();
}

void enterWinscreen() {
	UBYTE tile, tmp;
	initWinscreen();

	fadeFromWhite(10U);

	disable_interrupts();
	// Did we come from end of ending sequence?
	if(ending_flags & ENDING_FLAG_ENDING_FINISHED) {
		mus_setPaused(0U);
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

	while(1U) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			break;
		}
		wait_vbl_done();
	}

	winscreenShowRank();

	while(1U) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			if(unlocked_bits) {
				gamestate = GAMESTATE_UNLOCKED;
			} else {
				gamestate = GAMESTATE_HIGHSCORE;
			}
			break;
		}
		snd_update();
		wait_vbl_done();
	}

	clearSprites();
	fadeToWhite(10U);
	stopMusic();
}
