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

void drawTitleSprites(UBYTE triggered) {
	UBYTE i, j, frame;

	// Draw PUSH START
	if((!triggered && ticks < 60U) || (triggered && ticks & 4U)) {
		for(i = 0U; i != 11U; ++i) {
			if(i != 5U) {
				setSprite(48U + (i << 3), 140U, title_message[i], OBJ_PAL0);
			}
		}
	}

	// Draw cat
	frame = 37U;
	if(ticks < 20U || (ticks >= 40U && ticks < 60U)) frame += 6U;

	setSprite(player_x, player_y, frame++, OBJ_PAL0);
	setSprite(player_x+8, player_y, frame++, OBJ_PAL0);

	for(j = 1U; j != 3U; ++j) {
		for(i = 0U; i != 2U; ++i) {
			if(player_xdir == LEFT) {
				setSprite(player_x+(i<<3U), player_y+(j<<3U), frame, OBJ_PAL0);
			} else {
				setSprite(player_x+8U-(i<<3U), player_y+(j<<3U), frame, FLIP_X | OBJ_PAL0);
			}
			++frame;
		}
	}
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
	initTitle();

	fadeFromWhite(6U);

	selection = level = 1U;
	cheat_offset = 0U;

	player_x = 220U;
	player_y = 30U;
	player_xdir = LEFT;
	player_ydir = DOWN;
	player_xspeed = 128U;
	player_yspeed = 128U;
	scroll_x = 0U;
	scroll_y = 0U;
	elapsed_time = 0U;

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
			stopMusic();
			playSound(SFX_MENU_CONFIRM);
			
			for(ticks = 0U; ticks != 32U; ++ticks) {
				drawTitleSprites(1U);
				clearRemainingSprites();
				snd_update();
				wait_vbl_done();
			}

			gamestate = GAMESTATE_SELECT;
			break;
		}

		ticks++;
		if(ticks == 80U) {
			ticks = 0U;
			elapsed_time++;
		}

		if(elapsed_time == 25U) {
			gamestate = GAMESTATE_INTRO;
			stopMusic();
			break;
		}

		// Update cat physics
		if(player_xspeed < 128U) {
			scroll_x += (128U - player_xspeed) >> 4U;
		} else {
			scroll_x += (player_xspeed - 128U) >> 4U;
		}
		if(scroll_x >> 3U) {
			if(player_xspeed < 128U) player_x--;
			else player_x++;
			scroll_x &= 7U;
		}

		if(player_yspeed < 128U) {
			scroll_y += (128U - player_yspeed) >> 4U;
		} else {
			scroll_y += (player_yspeed - 128U) >> 4U;
		}
		if(scroll_y >> 3U) {
			if(player_yspeed < 128U) player_y--;
			else player_y++;
			scroll_y &= 7U;
		}

		if(player_ydir == DOWN) {
			player_xspeed = 96U;
			if(player_x < 154U && ISDOWN(J_LEFT | J_RIGHT | J_UP | J_DOWN)) {
				player_ydir = UP;
			}
		} else {
			if(ISDOWN(J_LEFT)) { player_xspeed -= 2U; player_xdir = LEFT; }
			else if(ISDOWN(J_RIGHT)) { player_xspeed += 2U; player_xdir = RIGHT; }
			else {
				if(player_xspeed < 128U) player_xspeed++;
				else if(player_xspeed > 128U) player_xspeed--;
			}

			if(ISDOWN(J_UP)) player_yspeed -= 2U;
			else if(ISDOWN(J_DOWN)) player_yspeed += 2U;
			else {
				if(player_yspeed < 128U) player_yspeed ++;
				else if(player_yspeed > 128U) player_yspeed--;
			}
		}

		if(player_xspeed < 32U) player_xspeed = 32U;
		else if(player_xspeed > 224U) player_xspeed = 224U;
		if(player_yspeed < 32U) player_yspeed = 32U;
		else if(player_yspeed > 224U) player_yspeed = 224U;

		drawTitleSprites(0U);
		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(6U);
	wait_sound_done();
}
