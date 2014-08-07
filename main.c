#include <gb/gb.h>
#include <rand.h>
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

	player.x = 80U;
	player.y = 0U;
	player.ydir = DOWN;
	player.yspeed = 0U;

	for(i = 0U; i < NUM_ENEMIES; ++i) {
		spawnEnemy();
	}
}

void updateInput() {
	UBYTE joystate;
	joystate = joypad();
	if(joystate & J_LEFT) {
		player.x -= MOVE_SPEED;
	}
	if(joystate & J_RIGHT) {
		player.x += MOVE_SPEED;
	}
}

void updatePlayer() {
	UBYTE i;
	// Left and right borders
	if(player.x < 16U) player.x = 16U;
	else if(player.x > 144U) player.x = 144U;

	// Bounce on water
	if(player.y >= 100U && player.y <= 108U) {
		for(i = 0U; i < NUM_ENEMIES; ++i) {
			if(enemy[i].type != ENEMY_NONE
			&& player.x > enemy[i].x-12U && player.x < enemy[i].x+12U) {
				player.y = 100U;
				player.ydir = UP;
				player.yspeed = JUMP_SPEED;
				killEnemy(i);
				spawnEnemy();
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
	if(player.ydir == UP) {
		if(player.y > 70U) {
			set_sprite_tile(0, 8);
			set_sprite_tile(1, 10);
		} else {
			set_sprite_tile(0, 0);
			set_sprite_tile(1, 2);
		}
	} else {
		set_sprite_tile(0, 4);
		set_sprite_tile(1, 6);
	}

	// Move player sprite
	move_sprite(0, player.x, player.y+16U);
	move_sprite(1, player.x+8U, player.y+16U);
}

void updateEnemies() {
	UBYTE i;
	for(i = 0U; i < NUM_ENEMIES; ++i) {
		if(enemy[i].type == ENEMY_JUMP) {
			move_sprite(2U*(i+1U), enemy[i].x, enemy[i].y+16U);
			move_sprite(2U*(i+1U)+1U, enemy[i].x+8U, enemy[i].y+16U);
		}
	}
}

void spawnEnemy() {
	UBYTE i;
	for(i = 0U; i < NUM_ENEMIES; ++i) {
		if(enemy[i].type == ENEMY_NONE) {
			break;
		}
	}
	if(i == NUM_ENEMIES) return;

	enemy[i].type = ENEMY_JUMP;
	enemy[i].x = 16U + i*32U;
	enemy[i].y = 116U;
	enemy[i].dir = (UBYTE)rand() % 1U;

	set_sprite_tile((i+1)*2U, 12);
	set_sprite_tile((i+1)*2U+1U, 14);
}

void killEnemy(UBYTE i) {
	enemy[i].type = ENEMY_NONE;
	move_sprite(2U*(i+1U), 0, 0);
	move_sprite(2U*(i+1U)+1U, 0, 0);
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
	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);

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
