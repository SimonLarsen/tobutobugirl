#include <gb/gb.h>
#include "defines.h"
#include "jukebox.h"
#include "fade.h"
#include "gamestate.h"
#include "cos.h"

#include "data/bg/jukebox.h"
//#include "data/sprite/characters.h"
#include "data/sprite/digital.h"
#include "data/sprite/arrow.h"

UBYTE jukebox_selection;

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
	{19U, 24U, 30U, 28U, 25U, 10U},
	{30U, 19U, 30U, 22U, 15U, 10U},
	{10U, 23U, 15U, 24U, 31U, 10U},
	{29U, 13U, 25U, 28U, 15U, 29U},
	{26U, 22U, 11U, 19U, 24U, 29U},
	{13U, 22U, 25U, 31U, 14U, 29U},
	{29U, 13U, 25U, 28U, 15U, 10U},
	{30U, 11U, 22U, 22U, 35U, 10U}
};

void initJukebox() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_sprite_data(0U, arrow_data_length, arrow_data);

	set_bkg_data(0U, 37U, digital_data);
	set_bkg_data_rle(jukebox_offset, jukebox_data_length, jukebox_data);
	set_bkg_tiles_rle(0U, 0U, jukebox_tiles_width, jukebox_tiles_height, jukebox_tiles);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	ticks = 0U;
	jukebox_selection = 0U;
	jukeboxUpdateTitle();

	clearSprites();

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	DISPLAY_ON;
	enable_interrupts();
}

void jukeboxUpdateSprites() {
	UBYTE offset;
	offset = cos32_64[(ticks & 63U)] >> 3;

	setSprite(36U-offset, 108U, 0U, OBJ_PAL0);
	setSprite(44U-offset, 108U, 2U, OBJ_PAL0);

	setSprite(124U+offset, 108U, 2U, OBJ_PAL0 | FLIP_X);
	setSprite(132U+offset, 108U, 0U, OBJ_PAL0 | FLIP_X);
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
}
