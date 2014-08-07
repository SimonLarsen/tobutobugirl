#include <gb/gb.h>
#include <rand.h>
#include "binconst.h"
#include "defines.h"
#include "main.h"

// Maps
#include "data/bg/background.h"
#include "data/bg/window.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE time, bkg_scroll;
struct Player player;

struct Enemy water_enemy[NUM_WATER];
struct Enemy low_enemy[NUM_LOW];
struct Enemy high_enemy[NUM_HIGH];
UBYTE enemy_count[ENEMY_TYPES];

void initIngame() {
	UBYTE i;

	time = 0U;
	bkg_scroll = 0U;

	set_sprite_tile(0U, 0U);
	set_sprite_tile(1U, 2U);
	set_sprite_prop(0U, B8(00010000));
	set_sprite_prop(1U, B8(00010000));
	player.x = 80U;
	player.y = 0U;
	player.xdir = RIGHT;
	player.ydir = DOWN;
	player.yspeed = 0U;
	player.jumped = 0U;

	for(i = 0U; i < ENEMY_TYPES; ++i) {
		enemy_count[i] = 0U;
	}
	for(i = 0U; i < 2U; ++i) {
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
	if(joystate & J_A && player.jumped == 0U && player.yspeed < JUMP_THRESHOLD) {
		player.ydir = UP;
		player.yspeed = JUMP_SPEED;
		player.jumped = 1U;
	}
}

void updatePlayer() {
	UBYTE i, frame;
	// Left and right borders
	if(player.x < 16U) player.x = 16U;
	else if(player.x > 144U) player.x = 144U;

	// Bounce on enemies
	if(player.y >= 108U && player.y <= 116U) {
		for(i = 0U; i < NUM_WATER; ++i) {
			if(water_enemy[i].type != ENEMY_NONE
			&& player.x > water_enemy[i].x-16U && player.x < water_enemy[i].x+16U) {
				bouncePlayer();
				spawnEnemy();
				killEnemy(&water_enemy[i]);
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
	set_sprite_tile(SPR_PLAYER, frame);
	set_sprite_tile(SPR_PLAYER+1U, frame+2U);

	// Move player sprite
	move_sprite(SPR_PLAYER, player.x, player.y+16U);
	move_sprite(SPR_PLAYER+1U, player.x+8U, player.y+16U);
}

void bouncePlayer() {
	player.y = 100U;
	player.ydir = UP;
	player.yspeed = JUMP_SPEED;
	player.jumped = 0U;
}

void updateEnemies() {
	UBYTE i, spr, offset;
	struct Enemy* e;

	for(i = 0U; i < NUM_WATER; ++i) {
		e = &water_enemy[i];
		e->frame++;
		switch(e->type) {
			case ENEMY_NONE:
				move_sprite(((SPR_WATER_E+i)<<1U), 0U, 0U);
				move_sprite(((SPR_WATER_E+i)<<1U) + 1U, 0U, 0U);
				break;

			case ENEMY_JUMP:
			case ENEMY_SPIKES:
				if(e->state < 3U) {
					if((e->frame & 7U) == 7U) {
						e->state++;
					}
				}
				else if((e->frame & 7U) == 7U) {
					e->state++;
					if(e->state == 5U) e->state = 3U;
				}
				offset = e->state << 2U;
				spr = (SPR_WATER_E+i) << 1U;
				if(e->type == ENEMY_JUMP) {
					set_sprite_tile(spr, 24U+offset);
					set_sprite_tile(spr+1U, 26U+offset);
				}
				else if(e->type == ENEMY_SPIKES) {
					set_sprite_tile(spr, 44U+offset);
					set_sprite_tile(spr+1U, 46U+offset);
				}
				move_sprite(spr, e->x, e->y+16U);
				move_sprite(spr+1U, e->x+8U, e->y+16U);
				break;
		}
	}
}

void spawnEnemy() {
	UBYTE i, j;
	struct Enemy* e;
	i = (UBYTE)rand() % NUM_WATER;
	for(j = 0U; j < NUM_WATER; ++j) {
		if(water_enemy[(i+j) % NUM_WATER].type == ENEMY_NONE) {
			i = (i+j) % NUM_WATER;
			break;
		}
	}
	if(j == NUM_WATER) return;

	e = &water_enemy[i];
	if(enemy_count[ENEMY_SPIKES] < 1U) {
		e->type = ENEMY_SPIKES;
		enemy_count[ENEMY_SPIKES]++;
	} else {
		e->type = ENEMY_JUMP;
		enemy_count[ENEMY_JUMP]++;
	}
	e->x = 16U + i*32U;
	e->y = 122U;
	e->dir = (UBYTE)rand() % 1U;
	e->state = 0U;
	e->frame = 0U;
}

void killEnemy(struct Enemy *e) {
	enemy_count[e->type]--;
	e->type = ENEMY_NONE;
}

void main() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = B8(11100100);
	OBP1_REG = B8(11010000);

	set_bkg_data(0, background_data_length, background_data);
	set_bkg_data(background_data_length, window_data_length, window_data);
	set_bkg_tiles(0, 0, background_tiles_width, background_tiles_height, background_tiles);
	set_win_tiles(0, 0, window_tiles_width, window_tiles_height, window_tiles);
	move_win(7U, 72U);

	set_sprite_data(0, sprites_data_length, sprites_data);

	SHOW_BKG;
	SHOW_SPRITES;
	SHOW_WIN;
	DISPLAY_ON;
	enable_interrupts();

	initIngame();
	spawnEnemy();

	while(1) {
		time++;
		if((time & 31U) == 31U) {
			bkg_scroll++;
			move_bkg(bkg_scroll, 0U);
		}
		updateEnemies();
		updateInput();
		updatePlayer();
		wait_vbl_done();
	}
}
