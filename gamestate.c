#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "game_backgrounds.h"
#include "cloud_animations.h"
#include "mmlgb/driver/music.h"

UBYTE ticks, paused;
UBYTE gamestate, scene_state;
UBYTE level, levels_completed;
UBYTE selection, sub_selection;
UBYTE joystate, oldjoystate;
UBYTE next_sprite, sprites_used;
UBYTE elapsed_time, remaining_time, kills;
UBYTE last_highscore_level, last_highscore_slot;
UBYTE last_progress;
UBYTE game_bank, music_bank;
UBYTE ending_finished;
UBYTE unlocked_bits;
UBYTE scroll_x, scroll_y;
UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_xspeed, player_yspeed, player_bounce;
UBYTE cat_x, cat_y, cat_frame;
UBYTE arrow_offset1, arrow_offset2;
UBYTE player_skin;
UBYTE show_dashcounter;
UBYTE circle_index;

const UBYTE level_names[7][6] = {
	{22U, 25U, 13U, 21U, 15U, 14U}, // "LOCKED"
	{26U, 22U, 11U, 19U, 24U, 29U}, // "PLAINS"
	{13U, 22U, 25U, 31U, 14U, 29U}, // "CLOUDS"
	{29U, 26U, 11U, 13U, 15U, 10U}, // "SPACE "
	{14U, 28U, 15U, 11U, 23U, 10U}, // "DREAM "
	{23U, 31U, 29U, 19U, 13U, 10U}, // "MUSIC "
	{29U, 13U, 25U, 28U, 15U, 29U}  // "SCORES"
};

const UBYTE level_max_time[4] = { 56U, 56U, 64U, 64U };

void setGameBank(UBYTE i) {
	game_bank = i;
	SWITCH_ROM_MBC1(i);
}

void setMusicBank(UBYTE i) {
	music_bank = i;
}

void playMusic(UBYTE *data) {
	SWITCH_ROM_MBC1(music_bank);
	mus_init(data);
	SWITCH_ROM_MBC1(game_bank);
}

void updateMusic() {
	SWITCH_ROM_MBC1(music_bank);
	mus_update();
	SWITCH_ROM_MBC1(game_bank);
}

void stopMusic() {
	mus_setPaused(1U);
}

void clearSprites() {
	UBYTE i;
	for(i = 0U; i < 40U; ++i) move_sprite(i, 0U, 0U);
	sprites_used = 0U;
	next_sprite = 0U;
}

void updateJoystate() {
	oldjoystate = joystate;
	joystate = joypad();
}

void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop) {
	move_sprite(next_sprite, x, y);
	set_sprite_tile(next_sprite, tile);
	set_sprite_prop(next_sprite, prop);

	sprites_used++;
	next_sprite++;
	if(next_sprite == 40U) next_sprite = 0U;
}

void clearRemainingSprites() {
	for(; sprites_used != 40U; ++sprites_used) {
		move_sprite(next_sprite++, 0, 0);
		if(next_sprite == 40U) next_sprite = 0U;
	}
	sprites_used = 0U;
}

void setIngameBackground(UBYTE level) {
	SWITCH_ROM_MBC1(7U);

	switch(level) {
		case 1U:
			set_bkg_data_rle(background1_offset, background1_data_length, background1_data);
			set_bkg_tiles_rle(0U, 0U, background1_tiles_width, background1_tiles_height, background1_tiles);
			break;
		case 2U:
			set_bkg_data_rle(background2_offset, background2_data_length, background2_data);
			set_bkg_tiles_rle(0U, 0U, background2_tiles_width, background2_tiles_height, background2_tiles);
			break;
		case 3U:
			set_bkg_data_rle(background3_offset, background3_data_length, background3_data);
			set_bkg_tiles_rle(0U, 0U, background3_tiles_width, background3_tiles_height, background3_tiles);
			break;
		case 4U:
			set_bkg_data_rle(background4_offset, background4_data_length, background4_data);
			set_bkg_tiles_rle(0U, 0U, background4_tiles_width, background4_tiles_height, background4_tiles);
			break;

		case 255U: // pause menu
			set_bkg_data_rle(pause_bg_offset, pause_bg_data_length, pause_bg_data);
			set_bkg_tiles_rle(0U, 0U, pause_bg_tiles_width, pause_bg_tiles_height, pause_bg_tiles);
			break;
	}

	SWITCH_ROM_MBC1(game_bank);
}

void setCloudAnimation(UBYTE skin) {
	SWITCH_ROM_MBC1(8U);

	switch(skin) {
		case 1U:
			set_sprite_data(0U, pause_cloud1_data_length, pause_cloud1_data);
			break;
		case 2U:
			set_sprite_data(0U, pause_cloud2_data_length, pause_cloud2_data);
			break;
	}

	SWITCH_ROM_MBC1(game_bank);
}

void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data) {
	UBYTE i, j, run, tile;
	UBYTE block[32];

	run = 0U;
	for(i = first; i != first+n; ++i) {
		for(j = 0U; j != 16U; ++j) {
			if(run == 0U) {
				tile = data[0];
				if(data[0] == data[1]) {
					run = data[2];
					data += 3U;
				} else {
					run = 1U;
					data++;
				}
			}
			block[j] = tile;
			run--;
		}
		set_bkg_data(i, 1U, block);
	}
}

void set_bkg_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles) {
	UBYTE ix, iy, run, tile;
	run = 0U;
	for(iy = y; iy != y+height; ++iy) {
		for(ix = x; ix != x+width; ++ix) {
			if(run == 0U) {
				tile = tiles[0];
				if(tiles[0] == tiles[1]) {
					run = tiles[2];
					tiles += 3U;
				} else {
					run = 1U;
					tiles++;
				}
			}
			set_bkg_tiles(ix, iy, 1U, 1U, &tile);
			run--;
		}
	}
}
