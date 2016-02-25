#include <gb/gb.h>
#include <rand.h>
#include "defines.h"
#include "title.h"
#include "fade.h"
#include "gamestate.h"
#include "sound.h"

#include "data/bg/titlescreen.h"
#include "data/sprite/characters.h"
#include "data/sprite/title_cat.h"

extern UBYTE title_song_data;

const UBYTE title_message[11] = {
	26U, 28U, 15U, 29U, 29U, 10U, 29U, 30U, 11U, 28U, 30U
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

	set_bkg_data(0U, titlescreen_data_length, titlescreen_data);
	set_bkg_tiles(0U, 0U, titlescreen_tiles_width, titlescreen_tiles_height, titlescreen_tiles);
	set_sprite_data(0U, 37U, characters_data);
	set_sprite_data(37U, title_cat_data_length, title_cat_data);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U;  // 11100100

	clearSprites();

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x8;

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

	levels_completed = 4U;
}

void enterTitle() {
	UBYTE i, j, frame;

	initTitle();

	fadeFromWhite(6U);

	selection = level = 1U;
	cheat_offset = 0U;

	player_x = 80U;
	player_y = 50U;
	player_xdir = LEFT;
	player_ydir = DOWN;
	player_xspeed = 0U;
	player_yspeed = 0U;
	scroll_x = 0U;
	scroll_y = 0U;

	setMusicBank(4U);
	disable_interrupts();
	playMusic(&title_song_data);
	enable_interrupts();

	ticks = 0U;
	while(1U) {
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
			playSound(SFX_MENU_CONFIRM);
			
			for(ticks = 0U; ticks != 32U; ++ticks) {
				if(ticks & 4U) {
					for(i = 0U; i != 11U; ++i) {
						if(i != 5U) {
							setSprite(48U + (i << 3), 136U, title_message[i], OBJ_PAL0);
						}
					}
				}

				snd_update();
				clearRemainingSprites();
				wait_vbl_done();
			}

			gamestate = GAMESTATE_SELECT;
			break;
		}

		// Draw PUSH START
		if(ticks < 60U) {
			for(i = 0U; i != 11U; ++i) {
				if(i != 5U) {
					setSprite(48U + (i << 3), 136U, title_message[i], OBJ_PAL0);
				}
			}
		}
		ticks++;
		if(ticks == 80U) ticks = 0U;

		// Update cat physics
		scroll_x += player_xspeed >> 4U;
		if(scroll_x >> 3U) {
			player_x = player_x + 3U - player_xdir;
			scroll_x &= 7U;
		}

		scroll_y += player_yspeed >> 4U;
		if(scroll_y >> 3U) {
			player_y = player_y - 2U + player_ydir;
			scroll_y &= 7U;
		}

		if(ISDOWN(J_LEFT)) {
			if(player_xdir == RIGHT) {
				if(player_xspeed >= 2U) player_xspeed -= 2U;
				else player_xdir = LEFT;
			} else {
				player_xspeed += 2U;
			}
		} else if(ISDOWN(J_RIGHT)) {
			if(player_xdir == LEFT) {
				if(player_xspeed >= 2U) player_xspeed -= 2U;
				else player_xdir = RIGHT;
			} else {
				player_xspeed += 2U;
			}
		} else {
			if(player_xspeed) player_xspeed--;
		}

		if(ISDOWN(J_UP)) {
			if(player_ydir == DOWN) {
				if(player_yspeed >= 2U) player_yspeed -= 2U;
				else player_ydir = UP;
			} else {
				player_yspeed += 2U;
			}
		} else if(ISDOWN(J_DOWN)) {
			if(player_ydir == UP) {
				if(player_yspeed >= 2U) player_yspeed -= 2U;
				else player_ydir = DOWN;
			} else {
				player_yspeed += 2U;
			}
		} else {
			if(player_yspeed) player_yspeed--;
		}

		if(player_xspeed > 100U) player_xspeed = 100U;
		if(player_yspeed > 100U) player_yspeed = 100U;

		// Draw cat
		frame = 37U;
		if(ticks < 20U || (ticks >= 40U && ticks < 60U)) frame += 6U;

		for(j = 0U; j != 3U; ++j) {
			for(i = 0U; i != 2U; ++i) {
				if(player_xdir == LEFT) {
					setSprite(player_x+(i<<3U), player_y+(j<<3U), frame, OBJ_PAL0);
				} else {
					setSprite(player_x+8U-(i<<3U), player_y+(j<<3U), frame, FLIP_X | OBJ_PAL0);
				}
				++frame;
			}
		}

		clearRemainingSprites();
		wait_vbl_done();
	}

	stopMusic();

	clearRemainingSprites();
	fadeToWhite(6U);
	wait_sound_done();
}
