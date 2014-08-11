#include <gb/gb.h>
#include <rand.h>
#include "binconst.h"
#include "defines.h"
#include "fade.h"
#include "input.h"
#include "gamestate.h"

#include "game.h"

// Levels
#include "levels.h"
// Maps
#include "data/bg/background.h"
#include "data/bg/window.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE time, loop, dead;
UBYTE enemies;
UBYTE scrolly, scrollx;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_jumped, player_bounce;

UBYTE cloud_x, cloud_y, cloud_frame;

UBYTE entity_x[NUM_ENEMIES];
UBYTE entity_y[NUM_ENEMIES];
UBYTE entity_type[NUM_ENEMIES];
UBYTE entity_dir[NUM_ENEMIES];
UBYTE entity_frame;

#define IS_KILLABLE(x) (x != E_NONE && x <= LAST_FRUIT && x != E_SPIKES)

const UBYTE entity_sprites[] = {
	0U,		// E_NONE
	24U,	// E_SEAL
	32U,	// E_SHIELD
	40U,	// E_BIRD
	56U,	// E_SPIKES
	64U,	// E_BAT
	72U,	// E_GRAPES
	76U,	// E_BANANA
	80U,	// E_PEACH
	84U,	// E_CLOUD
	104U,	// E_DOOR
};

const UBYTE entity_palette[] = {
	0U,	// E_NONE
	0U,	// E_SEAL
	0U,	// E_SHIELD
	1U,	// E_BIRD
	1U,	// E_SPIKES
	1U,	// E_BAT
	0U,	// E_GRAPES
	0U,	// E_BANANA
	0U,	// E_PEACH
	0U, // E_CLOUD
	0U, // E_DOOR
};

void gameIntro() {
	UBYTE tmp;

	HIDE_SPRITES;
	fadeFromWhite();
	SHOW_SPRITES;

	time = 0U;
	player_y = 200U;

	while(scrolly != 0U) {
		if(time & 7U == 7U) player_y--;
		updateEnemies(0U);

		if(player_y < SCRLMGN) scrolly = 0;
		else if(player_y > SCRLBTM) scrolly = 112U;
		else scrolly = player_y - SCRLMGN;

		tmp = 112U - scrolly;
		move_bkg(scrollx, 112U - (tmp >> 1U) - (tmp >> 2U));

		if(player_y > SCRLBTM) move_win(7U, 72U);
		else move_win(7U, (72U+SCRLBTM)-player_y);

		updateJoystate();
		if(CLICKED(J_START)) scrolly = 0U;

		time++;
		wait_vbl_done();
	}

	player_y = 0U;
	time = 0U;
}

void deathAnimation() {
	bouncePlayer();
	set_sprite_tile(SPR_PLAYER, 112U);
	set_sprite_tile(SPR_PLAYER+1U, 114U);
	
	time = 0U;
	while(time != 86U) {
		// Flying UP
		if(player_ydir == UP) {
			player_yspeed--;
			if(player_yspeed == 0U) {
				player_ydir = DOWN;
			}
			player_y -= (player_yspeed / 7U);
		}
		// Flying DOWN
		else {
			player_yspeed++;
			player_y += (player_yspeed / 7U);
			if(player_yspeed > MAX_YSPEED) {
				player_yspeed = MAX_YSPEED;
			}
		}

		move_sprite(SPR_PLAYER, player_x, player_y-scrolly+16U);
		move_sprite(SPR_PLAYER+1U, player_x+8U, player_y-scrolly+16U);

		time++;
		wait_vbl_done();
	}
}

void initGame() {
	UBYTE i;

	for(i = 0U; i < NUM_ENEMIES; ++i) killEntity(i);

	enemies = 0U;
	for(i = 0U; i < MAX_ENTITIES; ++i) {
		spawnEntity(i, levels[level][i][0], levels[level][i][1], levels[level][i][2], levels[level][i][3]);
	}

	time = 0U;
	scrollx = 0U;
	scrolly = 112U;

	set_sprite_tile(0U, 0U);
	set_sprite_tile(1U, 2U);
	set_sprite_prop(0U, B8(00010000));
	set_sprite_prop(1U, B8(00010000));

	player_x = 80U;
	player_y = 16U;
	player_xdir = RIGHT;
	player_ydir = DOWN;
	player_yspeed = 0U;
	player_jumped = 0U;
	player_bounce = 0U;
	dead = 0U;
	move_sprite(SPR_PLAYER, 168U, 0U);
	move_sprite(SPR_PLAYER+1U, 168U, 0U);

	cloud_frame = 5U;
	entity_frame = 0U;
}

void updateInput() {
	updateJoystate();

	if(HELD(J_LEFT)) {
		player_x -= MOVE_SPEED;
		player_xdir = LEFT;
	}
	if(HELD(J_RIGHT)) {
		player_x += MOVE_SPEED;
		player_xdir = RIGHT;
	}
	if(CLICKED(J_A) && player_jumped == 0U) {
		player_ydir = UP;
		player_yspeed = DJUMP_SPEED;
		player_jumped = 1U;
		setCloud(player_x, player_y+5U);
	}
}

void updatePlayer() {
	UBYTE i, frame;
	// Left and right borders
	if(player_x < 16U) player_x = 16U;
	else if(player_x > 144U) player_x = 144U;

	for(i = 0U; i < NUM_ENEMIES; ++i) {
		if(entity_type[i] != E_NONE
		&& player_x > entity_x[i]-14U && player_x < entity_x[i]+14U
		&& player_y > entity_y[i]-16U && player_y < entity_y[i]+13U) {
			if(entity_type[i] == E_SPIKES) {
				killPlayer();
			}
			else if(entity_type[i] == E_DOOR) {
				if(enemies == 0U) {
					loop = 0U;
				}
			}
			else if(entity_type[i] < FIRST_FRUIT) {
				if(player_ydir == DOWN && player_y < entity_y[i]-8U) {
					bouncePlayer();
					killEntity(i);
					setCloud(player_x, player_y+5U);
				}
				else {
					killPlayer();
				}
			} else {
				killEntity(i);
			}
			break;
		}
	}

	// Check water
	if(player_y > 246U) {
		killPlayer();
	}
	// Check top of screen
	if(player_y < 4U) player_y = 4U;

	// Flying UP
	if(player_ydir == UP) {
		player_yspeed--;
		if(player_yspeed == 0U) {
			player_ydir = DOWN;
		}
		player_y -= (player_yspeed / 7U);
	}
	// Flying DOWN
	else {
		player_yspeed++;
		player_y += (player_yspeed / 7U);
		if(player_yspeed > MAX_YSPEED) {
			player_yspeed = MAX_YSPEED;
		}
	}

	// Update sprite
	frame = 0U;
	if(player_xdir == RIGHT) frame = 12U;
	if(player_bounce != 0U) {
		frame += 8U;
		player_bounce--;
	}
	else if(player_ydir == DOWN) {
		frame += 4;
	}

	// Update sprite
	set_sprite_tile(SPR_PLAYER, frame);
	set_sprite_tile(SPR_PLAYER+1U, frame+2U);

	// Move player sprite
	move_sprite(SPR_PLAYER, player_x, player_y-scrolly+16U);
	move_sprite(SPR_PLAYER+1U, player_x+8U, player_y-scrolly+16U);

	// Update cloud
	if(cloud_frame != 5U && (time & 3U) == 3U) cloud_frame++;

	if(cloud_frame != 5U) {
		frame = entity_sprites[E_CLOUD] + (cloud_frame << 2U);

		move_sprite(SPR_CLOUD, cloud_x, cloud_y-scrolly+16U);
		move_sprite(SPR_CLOUD+1U, cloud_x+8U, cloud_y-scrolly+16U);

		set_sprite_tile(SPR_CLOUD, frame);
		set_sprite_tile(SPR_CLOUD+1U, frame+2U);
	} else {
		move_sprite(SPR_CLOUD, 168U, 0U);
		move_sprite(SPR_CLOUD+1U, 168U, 0U);
	}
}

void bouncePlayer() {
	player_ydir = UP;
	player_yspeed = JUMP_SPEED;
	player_jumped = 0U;
	player_bounce = 16U;
}

void killPlayer() {
	dead = 1U;
}

void setCloud(UBYTE x, UBYTE y) {
	cloud_x = x;
	cloud_y = y;
	cloud_frame = 0U;

	move_sprite(SPR_CLOUD, x, y-scrolly+16U);
	move_sprite(SPR_CLOUD+1U, x+8U, y-scrolly+16U);

	set_sprite_tile(SPR_CLOUD, entity_sprites[E_CLOUD]);
	set_sprite_tile(SPR_CLOUD+1U, entity_sprites[E_CLOUD]+2U);
}

void updateEnemies(UBYTE move) {
	UBYTE i, sprite, frame;

	if((time & 7U) == 7U) entity_frame++;

	for(i = 0U; i < NUM_ENEMIES; ++i) {
		switch(entity_type[i]) {
			case E_BIRD:
				if((time & 1U) && move) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] == 168U) entity_x[i] = 248U;
					}
					else {
						entity_x[i]--;
						if(entity_x[i] == 248U) entity_x[i] = 168U;
					}
				}
				break;
		}

		frame = entity_sprites[entity_type[i]];
		if(entity_type[i] < FIRST_FRUIT && entity_frame & 1U) frame += 4U;
		else if(entity_type[i] == E_DOOR && enemies == 0U) frame += 4U;
		if(entity_dir[i] == RIGHT) frame += 8U;
		
		sprite = SPR_ENEMIES + (i << 1U);
		set_sprite_tile(sprite, frame);
		set_sprite_tile(sprite+1U, frame+2U);

		move_sprite(sprite, entity_x[i], entity_y[i]-scrolly+16U);
		move_sprite(sprite+1U, entity_x[i]+8U, entity_y[i]-scrolly+16U);
	}
}

void spawnEntity(UBYTE i, UBYTE type, UBYTE x, UBYTE y, UBYTE dir) {
	UBYTE palette, sprite;
	if(type == E_NONE) return;

	entity_x[i] = x;
	entity_y[i] = y;
	entity_type[i] = type;
	entity_dir[i] = dir;

	sprite = SPR_ENEMIES + (i << 1U);
	palette = entity_palette[type] << 4U;
	set_sprite_prop(sprite, palette);
	set_sprite_prop(sprite+1U, palette);

	if(IS_KILLABLE(type)) enemies++;
}

void killEntity(UBYTE i) {
	if(IS_KILLABLE(entity_type[i])) {
		enemies--;
	}
	entity_type[i] = E_NONE;
	entity_x[i] = 168U;
	entity_y[i] = 0U;
}

void updateScroll() {
	UBYTE tmp;

	if((time & 31U) == 31U) scrollx++;

	if(player_y < SCRLMGN) scrolly = 0;
	else if(player_y > SCRLBTM) scrolly = 112U;
	else scrolly = player_y - SCRLMGN;

	tmp = 112U - scrolly;
	move_bkg(scrollx, 112U - (tmp >> 1U) - (tmp >> 2U));

	if(player_y > SCRLBTM) move_win(7U, 72U);
	else move_win(7U, (72U+SCRLBTM)-player_y);
}

void enterGame() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = B8(11100100);
	OBP1_REG = B8(11010000);
	BGP_REG = B8(11100100);

	set_bkg_data(0U, background_data_length, background_data);
	set_bkg_data(background_data_length, window_data_length, window_data);
	set_bkg_tiles(0U, 0U, background_tiles_width, background_tiles_height, background_tiles);
	set_win_tiles(0U, 0U, window_tiles_width, window_tiles_height, window_tiles);

	move_bkg(0U, 112U);
	move_win(7U, 72U);

	set_sprite_data(0U, sprites_data_length, sprites_data);

	clearSprites();

	SHOW_BKG;
	SHOW_SPRITES;
	SHOW_WIN;
	DISPLAY_ON;
	enable_interrupts();

	initGame();

	gameIntro();

	loop = 1U;
	while(loop && !dead) {
		time++;

		updateScroll();

		updateEnemies(1U);
		updateInput();
		updatePlayer();

		if(CLICKED(J_SELECT)) {
			enemies = 1U;
			break;
		}

		wait_vbl_done();
	}

	if (dead) {
		deathAnimation();
		gamestate = GAMESTATE_GAME;
	}
	else if(enemies == 0U) {
		gamestate = GAMESTATE_LEVEL;
		completed[level] = 1U;
	}
	else {
		gamestate = GAMESTATE_LEVEL;
	}

	HIDE_SPRITES;
	fadeToWhite();
}
