#include <gb/gb.h>
#include "defines.h"
#include "jukebox.h"
#include "fade.h"
#include "gamestate.h"
#include "cos.h"

#include "data/bg/jukebox.h"
#include "data/sprite/digital.h"
#include "data/sprite/arrow.h"
#include "data/sprite/notes.h"
#include "data/sprite/bobblehead.h"

UBYTE jukebox_active;
UBYTE jukebox_selection;
UBYTE jukebox_music_ticks;
UBYTE jukebox_bop;

extern UBYTE intro1_song_data;    // bank 6
extern UBYTE title_song_data;     // bank 4
extern UBYTE mainmenu_song_data;  // bank 4
extern UBYTE plains_song_data;    // bank 5
extern UBYTE clouds_song_data;    // bank 5
extern UBYTE space_song_data;     // bank 5
extern UBYTE highscore_song_data; // bank 4
extern UBYTE winscreen_song_data; // bank 4
#define JUKEBOX_NUM_SONGS 8

const UBYTE song_names[JUKEBOX_NUM_SONGS][6] = {
	{19U, 24U, 30U, 28U, 25U, 10U}, // "INTRO "
	{30U, 19U, 30U, 22U, 15U, 10U}, // "TITLE "
	{10U, 23U, 15U, 24U, 31U, 10U}, // " MENU "
	{26U, 22U, 11U, 19U, 24U, 29U}, // "PLAINS"
	{13U, 22U, 25U, 31U, 14U, 29U}, // "CLOUDS"
	{29U, 26U, 11U, 13U, 15U, 10U}, // "SPACE "
	{29U, 13U, 25U, 28U, 15U, 29U}, // "SCORES"
	{30U, 11U, 22U, 22U, 35U, 10U}  // "TALLY "
};

void initJukebox() {
	disable_interrupts();
	DISPLAY_OFF;

	remove_TIM(updateMusic);
	add_TIM(jukeboxUpdateMusic);

	move_bkg(0U, 0U);

	set_sprite_data(0U, arrow_data_length, arrow_data);
	set_sprite_data(arrow_data_length, notes_data_length, notes_data);
	set_sprite_data(arrow_data_length+notes_data_length, bobblehead_data_length, bobblehead_data);

	set_bkg_data(0U, 37U, digital_data);
	set_bkg_data_rle(jukebox_offset, jukebox_data_length, jukebox_data);
	set_bkg_tiles_rle(0U, 0U, jukebox_tiles_width, jukebox_tiles_height, jukebox_tiles);

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xE4; // 11100100
	BGP_REG = 0xE4U; // 11100100

	ticks = 0U;
	jukebox_active = 0U;
	jukebox_selection = 0U;
	jukebox_music_ticks = 0U;
	jukeboxUpdateTitle();

	clearSprites();

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	DISPLAY_ON;
	enable_interrupts();
}

void jukeboxUpdateMusic() {
	if(!jukebox_active) return;

	updateMusic();
	jukebox_music_ticks++;
	if(jukebox_music_ticks == 24U) {
		jukebox_music_ticks = 0U;
		jukebox_bop++;
	}
}

void jukeboxUpdateSprites() {
	UBYTE offset;
	offset = cos32_64[(ticks & 63U)] >> 3;

	// Left arrow
	setSprite(36U-offset, 108U, 0U, OBJ_PAL0);
	setSprite(44U-offset, 108U, 2U, OBJ_PAL0);

	// Right arrow
	setSprite(124U+offset, 108U, 2U, OBJ_PAL0 | FLIP_X);
	setSprite(132U+offset, 108U, 0U, OBJ_PAL0 | FLIP_X);

	// Small note
	offset = jukebox_bop & 1U;
	setSprite(11U, 94U+offset, 4U, OBJ_PAL0);
	setSprite(145U, 18U+offset, 4U, OBJ_PAL0);

	// Double note
	offset = (jukebox_bop & 1U) ^ 1U;
	setSprite(12U, 19U+offset, 8U, OBJ_PAL0);
	setSprite(20U, 19U+offset, 10U, OBJ_PAL0);

	// Bobble head figurine
	offset = jukebox_bop & 3U;
	if(offset == 1U) {
		offset = 20U;
	} else if(offset == 3U) {
		offset = 44U;
	} else {
		offset = 32U;
	}
	if(offset == 3U) offset = 0U;
	setSprite(135U, 34U, offset, OBJ_PAL1);
	setSprite(143U, 34U, offset+2U, OBJ_PAL1);
	setSprite(151U, 34U, offset+4U, OBJ_PAL1);
	setSprite(135U, 50U, offset+6U, OBJ_PAL1);
	setSprite(143U, 50U, offset+8U, OBJ_PAL1);
	setSprite(151U, 50U, offset+10U, OBJ_PAL1);
}

void jukeboxUpdateTitle() {
	// 7,12
	set_bkg_tiles(7U, 12U, 6U, 1U, &song_names[jukebox_selection]);
}

void enterJukebox() {
	initJukebox();

	fadeFromWhite(10U);

	while(1U) {
		updateJoystate();

		ticks++;

		if(CLICKED(J_LEFT)) {
			if(jukebox_selection == 0U) {
				jukebox_selection = JUKEBOX_NUM_SONGS-1U;
			} else {
				jukebox_selection--;
			}
			jukeboxUpdateTitle();
		}
		if(CLICKED(J_RIGHT)) {
			if(jukebox_selection == JUKEBOX_NUM_SONGS-1U) {
				jukebox_selection = 0U;
			} else {
				jukebox_selection++;
			}
			jukeboxUpdateTitle();
		}
		if(CLICKED(J_A) || CLICKED(J_START)) {
			stopMusic();
			jukebox_music_ticks = 0U;
			switch(jukebox_selection) {
				case 0U:
					setMusicBank(6U);
					playMusic(&intro1_song_data);
					break;
				case 1U:
					setMusicBank(4U);
					playMusic(&title_song_data);
					break;
				case 2U:
					setMusicBank(4U);
					playMusic(&mainmenu_song_data);
					break;
				case 3U:
					setMusicBank(5U);
					playMusic(&plains_song_data);
					break;
				case 4U:
					setMusicBank(5U);
					playMusic(&clouds_song_data);
					break;
				case 5U:
					setMusicBank(5U);
					playMusic(&space_song_data);
					break;
				case 6U:
					setMusicBank(4U);
					playMusic(&highscore_song_data);
					break;
				case 7U:
					setMusicBank(4U);
					playMusic(&winscreen_song_data);
					break;
			}
			jukebox_active = 1U;
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
			break;
		}

		jukeboxUpdateSprites();
		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(10U);

	stopMusic();
	remove_TIM(jukeboxUpdateMusic);
	add_TIM(updateMusic);
}
