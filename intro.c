#include <gb/gb.h>
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "intro.h"

#include "data/bg/intro_bg.h"
#include "data/sprite/intro_sprites.h"
#include "data/sprite/intro_flash.h"

UBYTE intro_frame;

extern UBYTE intro1_song_data;

#define INTRO_STATE_WALK  1U
#define INTRO_STATE_TRIP  2U
#define INTRO_STATE_BLINK 3U

void initIntro() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 112U);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	set_bkg_data_rle(0U, intro_bg_data_length, intro_bg_data);
	set_bkg_tiles_rle(0U, 0U, intro_bg_tiles_width, intro_bg_tiles_height, intro_bg_tiles);

	set_sprite_data(0U, intro_sprites_data_length, intro_sprites_data);

	clearSprites();
	
	setMusicBank(6U);
	playMusic(&intro1_song_data);

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	DISPLAY_ON;
	enable_interrupts();
}

void updateIntro() {
	UBYTE frame;

	switch(scene_state) {
		case INTRO_STATE_WALK:
			if(ticks & 1U) {
				player_x--;
				cat_x--;
			}
			if((ticks & 7U) == 7U) {
				intro_frame++;
				if(intro_frame == 6U) intro_frame = 0U;
			}
			frame = intro_frame << 2;
			// Player sprites
			setSprite(player_x, player_y-scroll_y, frame, OBJ_PAL0);
			setSprite(player_x+8, player_y-scroll_y, frame+2U, OBJ_PAL0);

			// Cat sprites
			cat_y = player_y-12U + ((ticks & 16U) >> 4);
			setSprite(cat_x, cat_y-scroll_y-16U, 32U, OBJ_PAL0);
			setSprite(cat_x+8U, cat_y-scroll_y-16U, 34U, OBJ_PAL0);
			setSprite(cat_x, cat_y-scroll_y, 36U, OBJ_PAL0);
			setSprite(cat_x+8U, cat_y-scroll_y, 38U, OBJ_PAL0);

			if(player_x == 77U) {
				scene_state = INTRO_STATE_TRIP;
				player_y = 239U;
				ticks = 0U;
			}
			break;

		case INTRO_STATE_TRIP:
			if(ticks & 1U) {
				if(player_x < 55U) {
					if(cat_y >= 130U || (ticks & 3U) == 3U) {
						cat_y--;
					}
				}
				if(player_x != 45U) player_x--;
			}
			if((ticks & 3U) == 3U) {
				if(scroll_y && cat_y < scroll_y+64U) {
					scroll_y--;
				}
			}
			if((ticks & 7U) == 7U) {
				if(cat_y >= 130U || (ticks & 15U) == 15U) {
					cat_x++;
				}
			}
			if((ticks & 15U) == 15U) {
				if(player_x != 45U) player_y++;
			}
			if(player_x > 45U) {
				frame = 24U;
			} else {
				frame = 28U;
				player_y = 240U;
			}
			// Player sprites
			setSprite(player_x, player_y-scroll_y, frame, OBJ_PAL0);
			setSprite(player_x+8, player_y-scroll_y, frame+2U, OBJ_PAL0);

			// Player shocked mark
			if(player_x > 68U) {
				setSprite(player_x, player_y-scroll_y-13U, 52U, OBJ_PAL0);
				setSprite(player_x+8, player_y-scroll_y-13U, 54U, OBJ_PAL0);
			}

			// Exclamation mark
			if((ticks & 16U) && cat_y < 210U && cat_y > 180U) {
				setSprite(player_x+4U, player_y-scroll_y-13U, 50U, OBJ_PAL0);
			}

			frame = 56U;
			if(cat_y < 180U) {
				frame = 40U;
			}

			if(cat_y < 130U) {
				setSprite(cat_x+4U, cat_y-scroll_y-10U, 48U, OBJ_PAL0);
			} else {
				setSprite(cat_x, cat_y-scroll_y-16U, frame, OBJ_PAL0);
				setSprite(cat_x+8U, cat_y-scroll_y-16U, frame+2U, OBJ_PAL0);
				setSprite(cat_x, cat_y-scroll_y, frame+4U, OBJ_PAL0);
				setSprite(cat_x+8U, cat_y-scroll_y, frame+6U, OBJ_PAL0);
			}

			if(cat_y < 110U) {
				scene_state = INTRO_STATE_BLINK;
				intro_frame = 0U;
				set_sprite_data(0U, intro_flash_data_length, intro_flash_data);
			}

			break;

		case INTRO_STATE_BLINK:
			if((ticks & 3U) == 3U) {
				if(scroll_y) {
					scroll_y--;
				}
			}
			if((ticks & 3U) == 3U) {
				intro_frame++;
			}

			if(intro_frame == 0U) {
				setSprite(cat_x, cat_y-scroll_y-16U, 0U, OBJ_PAL0);
				setSprite(cat_x+8U, cat_y-scroll_y-16U, 2U, OBJ_PAL0);
				setSprite(cat_x+16U, cat_y-scroll_y-16U, 4U, OBJ_PAL0);
				setSprite(cat_x, cat_y-scroll_y, 6U, OBJ_PAL0);
				setSprite(cat_x+8U, cat_y-scroll_y, 8U, OBJ_PAL0);
				setSprite(cat_x+16U, cat_y-scroll_y, 10U, OBJ_PAL0);
			}
			else if(intro_frame == 1U) {
				setSprite(cat_x+9U, cat_y-scroll_y-4U, 12U, OBJ_PAL0);
			}
			else if(intro_frame == 2U) {
				setSprite(cat_x+4U, cat_y-scroll_y-8U, 16U, OBJ_PAL0);
				setSprite(cat_x+12U, cat_y-scroll_y-8U, 18U, OBJ_PAL0);
			}
			else if(intro_frame == 3U) {
				setSprite(cat_x-2U, cat_y-scroll_y-11U, 20U, OBJ_PAL0);
				setSprite(cat_x+2U, cat_y-scroll_y-11U, 22U, OBJ_PAL0);
				setSprite(cat_x+10U, cat_y-scroll_y-11U, 24U, OBJ_PAL0);
				setSprite(cat_x+18U, cat_y-scroll_y-11U, 26U, OBJ_PAL0);
			}
			else if(intro_frame == 4U) {
				setSprite(cat_x-2U, cat_y-scroll_y-11U, 28U, OBJ_PAL0);
				setSprite(cat_x+2U, cat_y-scroll_y-11U, 30U, OBJ_PAL0);
				setSprite(cat_x+10U, cat_y-scroll_y-11U, 32U, OBJ_PAL0);
				setSprite(cat_x+18U, cat_y-scroll_y-11U, 34U, OBJ_PAL0);
			}

			if(scroll_y == 16U) {
				gamestate = GAMESTATE_TITLE;
			}
			break;
	}
}

void enterIntro() {
	initIntro();

	fadeFromWhite(10U);

	scene_state = INTRO_STATE_WALK;
	scroll_y = 112U;

	player_x = 185U;
	player_y = 240U;
	cat_x = player_x + 13U;
	cat_y = 228U;

	intro_frame = 0U;

	ticks = 0U;
	while(gamestate == GAMESTATE_INTRO) {
		updateJoystate();

		updateIntro();
		ticks++;

		if(CLICKED(J_START)) {
			gamestate = GAMESTATE_TITLE;
		}

		clearRemainingSprites();
		wait_vbl_done();
		move_bkg(0U, scroll_y);
	}

	clearRemainingSprites();
	fadeToWhite(10U);

	stopMusic();
}
