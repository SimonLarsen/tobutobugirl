#include <gb/gb.h>
#include <rand.h>
#include "binconst.h"
#include "defines.h"
#include "main.h"

// Maps
#include "data/bg/background.h"
// Sprites
#include "data/sprite/sprites.h"

#define MOVE_SPEED 2U
#define JUMP_SPEED 28U
#define MAX_YSPEED 20U

#define NUM_ENEMIES 5U

struct Player {
	UBYTE x, y;
	UBYTE xdir;
	UBYTE ydir, yspeed;
};

struct Enemy {
	UBYTE type;
	UBYTE x, y, dir;
	UBYTE frame;
};

struct Player player;
struct Enemy enemy[NUM_ENEMIES];

void initIngame() {
	UBYTE i;

	set_sprite_tile(0U, 0U);
	set_sprite_tile(1U, 2U);
	player.x = 80U;
	player.y = 0U;
	player.xdir = RIGHT;
	player.ydir = DOWN;
	player.yspeed = 0U;

	for(i = 0U; i < 3U; ++i) {
		spawnEnemy();
	}
}

void updateInput() {
	UBYTE joystate;
	joystate = joypad();
	if(joystate & J_LEFT) {
		player.x -= MOVE_SPEED;
		player.xdir = LEFT;
	}
	if(joystate & J_RIGHT) {
		player.x += MOVE_SPEED;
		player.xdir = RIGHT;
	}
}

void updatePlayer() {
	UBYTE i, frame;
	// Left and right borders
	if(player.x < 16U) player.x = 16U;
	else if(player.x > 144U) player.x = 144U;

	// Bounce on water
	if(player.y >= 108U && player.y <= 116U) {
		for(i = 0U; i < NUM_ENEMIES; ++i) {
			if(enemy[i].type != ENEMY_NONE
			&& player.x > enemy[i].x-16U && player.x < enemy[i].x+16U) {
				player.y = 100U;
				player.ydir = UP;
				player.yspeed = JUMP_SPEED;
				spawnEnemy();
				killEnemy(i);
				break;
			}
		}
	}
	// Flying UP
	if(player.ydir == UP) {
		player.yspeed--;
		if(player.yspeed == 0U) {
			player.ydir = DOWN;
		}
		player.y -= (player.yspeed / 7U);
	}
	// Flying DOWN
	else {
		player.yspeed++;
		player.y += (player.yspeed / 7U);
		if(player.yspeed > MAX_YSPEED) {
			player.yspeed = MAX_YSPEED;
		}
	}

	// Update sprite
	frame = 0U;
	if(player.xdir == RIGHT) frame = 12U;
	if(player.ydir == UP) {
		if(player.y > 70U) {
			frame += 8U;
		}
	} else {
		frame += 4U;
	}

	// Update sprite
	set_sprite_tile(0U, frame);
	set_sprite_tile(1U, frame+2U);

	// Move player sprite
	move_sprite(0U, player.x, player.y+16U);
	move_sprite(1U, player.x+8U, player.y+16U);
}

void updateEnemies() {
	UBYTE i;
	for(i = 0U; i < NUM_ENEMIES; ++i) {
		if(enemy[i].type == ENEMY_JUMP) {
			move_sprite(((i+1U)<<1U), enemy[i].x, enemy[i].y+16U);
			move_sprite(((i+1U)<<1U) + 1U, enemy[i].x+8U, enemy[i].y+16U);
		}
	}
}

void spawnEnemy() {
	UBYTE i, j;
	i = (UBYTE)rand() % NUM_ENEMIES;
	for(j = 0U; j < NUM_ENEMIES; ++j) {
		if(enemy[(i+j) % NUM_ENEMIES].type == ENEMY_NONE) {
			i = (i+j) % NUM_ENEMIES;
			break;
		}
	}
	if(j == NUM_ENEMIES) return;

	enemy[i].type = ENEMY_JUMP;
	enemy[i].x = 16U + i*32U;
	enemy[i].y = 120U;
	enemy[i].dir = (UBYTE)rand() % 1U;

	set_sprite_tile(((i+1)<<1U), 24U);
	set_sprite_tile(((i+1)<<1U) + 1U, 26U);
}

void killEnemy(UBYTE i) {
	enemy[i].type = ENEMY_NONE;
	move_sprite(((i+1U)<<1U), 0U, 0U);
	move_sprite(((i+1U)<<1U)+1U, 0U, 0U);
}

void main() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	// Load BKG data
	set_bkg_data(0, background_data_length, background_data);
	set_bkg_tiles(0, 0, background_tiles_width, background_tiles_height, background_tiles);

	// Load sprite data
	set_sprite_data(0, sprites_data_length, sprites_data);

	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;
	enable_interrupts();

	initIngame();
	spawnEnemy();

	while(1) {
		updateEnemies();
		updateInput();
		updatePlayer();
		wait_vbl_done();
	}
}
