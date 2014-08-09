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

UBYTE scrolly, scrollx;
UBYTE time;
UBYTE joystate, oldjoystate;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_jumped, player_bounce;

UBYTE enemy_x[NUM_ENEMIES];
UBYTE enemy_y[NUM_ENEMIES];
UBYTE enemy_type[NUM_ENEMIES];
UBYTE enemy_sprite[NUM_ENEMIES];
UBYTE enemy_state[NUM_ENEMIES];
UBYTE enemy_frame[NUM_ENEMIES];
UBYTE enemy_dir[NUM_ENEMIES];

#define CLICKED(x) ((joystate & x) != (oldjoystate & x))
#define HELD(x) (joystate & x)

void initIngame() {
	UBYTE i;

	time = 0U;
	scrollx = 0U;
	scrolly = 112U;
	joystate = 0U;
	oldjoystate = 0U;

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

	for(i = 0U; i < NUM_ENEMIES; ++i) enemy_type[i] = ENEMY_NONE;

	for(i = SPR_LOW_E; i < SPR_LOW_E+NUM_LOW*2U; ++i) {
		set_sprite_prop(i, B8(00010000));
	}
	for(i = SPR_HIGH_E; i < SPR_HIGH_E+NUM_HIGH*2U; ++i) {
		set_sprite_prop(i, B8(00010000));
	}
	for(i = 0U; i < 3U; ++i) {
		spawnWaterEnemy();
	}

	for(i = NUM_WATER; i < NUM_WATER+NUM_LOW; ++i) {
		spawnLowEnemy();
		enemy_x[i] = (UBYTE)rand() % 160U;
	}
	for(i = NUM_WATER+NUM_LOW; i < NUM_ENEMIES; ++i) {
		spawnHighEnemy();
		enemy_x[i] = (UBYTE)rand() % 160U;
	}
}

void updateInput() {
	joystate = joypad();
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
	}

	oldjoystate = joystate;
}

void updatePlayer() {
	UBYTE i, frame;
	// Left and right borders
	if(player_x < 16U) player_x = 16U;
	else if(player_x > 144U) player_x = 144U;

	for(i = 0U; i < NUM_ENEMIES; ++i) {
		if(enemy_type[i] != ENEMY_NONE
		&& player_x > enemy_x[i]-16U && player_x < enemy_x[i]+16U
		&& player_y > enemy_y[i]-15U && player_y < enemy_y[i]-7U) {
			bouncePlayer();
			killEnemy(i);
			if(enemy_type[i] == ENEMY_JUMP || enemy_type[i] == ENEMY_SPIKES) {
					spawnWaterEnemy();
			}
			else if(enemy_type[i] == ENEMY_BIRD) {
				killEnemy(i);
				if(enemy_y[i] == LOW_Y) {
					spawnLowEnemy();
				} else {
					spawnHighEnemy();
				}
			}
			break;
		}
	}

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
}

void bouncePlayer() {
	player_ydir = UP;
	player_yspeed = JUMP_SPEED;
	player_jumped = 0U;
	player_bounce = 16U;
}

void updateEnemy(UBYTE i) {
	UBYTE offset;

	enemy_frame[i]++;
	switch(enemy_type[i]) {
		case ENEMY_NONE:
			move_sprite(enemy_sprite[i], 0U, 0U);
			move_sprite(enemy_sprite[i]+1U, 0U, 0U);
			break;

		case ENEMY_JUMP:
		case ENEMY_SPIKES:
			if(enemy_state[i] < 3U) {
				if((enemy_frame[i] & 7U) == 7U) {
					enemy_state[i]++;
				}
			}
			else if((enemy_frame[i] & 7U) == 7U) {
				enemy_state[i]++;
				if(enemy_state[i] == 5U) enemy_state[i] = 3U;
			}
			offset = enemy_state[i] << 2U;
			if(enemy_type[i] == ENEMY_JUMP) {
				set_sprite_tile(enemy_sprite[i], 24U+offset);
				set_sprite_tile(enemy_sprite[i]+1U, 26U+offset);
			}
			else if(enemy_type[i] == ENEMY_SPIKES) {
				set_sprite_tile(enemy_sprite[i], 44U+offset);
				set_sprite_tile(enemy_sprite[i]+1U, 46U+offset);
			}
			move_sprite(enemy_sprite[i], enemy_x[i], enemy_y[i]-scrolly+16U);
			move_sprite(enemy_sprite[i]+1U, enemy_x[i]+8U, enemy_y[i]-scrolly+16U);
			break;

		case ENEMY_BIRD:
			if(enemy_frame[i] & 1U) {
				if(enemy_dir[i] == RIGHT) {
					enemy_x[i]++;
					if(enemy_x[i] == 168U) enemy_x[i] = 248U;
				}
				else {
					enemy_x[i]--;
					if(enemy_x[i] == 248U) enemy_x[i] = 168U;
				}
			}

			if((enemy_frame[i] & 7U) == 7U) {
				enemy_state[i] = enemy_state[i] ^ 1U;
			}

			offset = 64U + (enemy_state[i] << 2U);
			if(enemy_dir[i] == RIGHT) offset += 8U;
			
			set_sprite_tile(enemy_sprite[i], offset);
			set_sprite_tile(enemy_sprite[i]+1U, offset+2U);

			move_sprite(enemy_sprite[i], enemy_x[i], enemy_y[i]-scrolly+16U);
			move_sprite(enemy_sprite[i]+1U, enemy_x[i]+8U, enemy_y[i]-scrolly+16U);
			break;
	}
}

void updateEnemies() {
	UBYTE i;

	for(i = 0; i < NUM_ENEMIES; ++i) {
		updateEnemy(i);
	}
}

void spawnWaterEnemy() {
	UBYTE i, j;
	i = (UBYTE)rand() % NUM_WATER;
	for(j = 0U; j < NUM_WATER; ++j) {
		if(enemy_type[(i+j) % NUM_WATER] == ENEMY_NONE) {
			i = (i+j) % NUM_WATER;
			break;
		}
	}
	if(j == NUM_WATER) return;

	enemy_type[i] = ENEMY_JUMP;
	enemy_sprite[i] = SPR_ENEMIES + (i << 1U);
	enemy_x[i] = 16U + i*32U;
	enemy_y[i] = WATER_Y;
	enemy_state[i] = 0U;
	enemy_frame[i] = 0U;
}

void spawnLowEnemy() {
	UBYTE i;

	for(i = NUM_WATER; i < NUM_WATER+NUM_LOW; ++i) {
		if(enemy_type[i] == ENEMY_NONE) {
			break;
		}
	}
	if(i == NUM_LOW) return;

	enemy_sprite[i] = SPR_ENEMIES + (i << 1U);
	enemy_type[i] = ENEMY_BIRD;
	enemy_state[i] = 0U;
	enemy_frame[i] = 0U;
	enemy_y[i] = LOW_Y;
	if((UBYTE)rand() & 1U) {
		enemy_x[i] = 240U;
		enemy_dir[i] = RIGHT;
	} else {
		enemy_x[i] = 176U;
		enemy_dir[i] = LEFT;
	}
}

void spawnHighEnemy() {
	UBYTE i;

	for(i = NUM_WATER+NUM_LOW; i < NUM_ENEMIES; ++i) {
		if(enemy_type[i] == ENEMY_NONE) {
			break;
		}
	}
	if(i == NUM_HIGH) return;

	enemy_sprite[i] = SPR_ENEMIES + (i << 1U);
	enemy_type[i] = ENEMY_BIRD;
	enemy_state[i] = 0U;
	enemy_frame[i] = 0U;
	enemy_y[i] = HIGH_Y;
	if((UBYTE)rand() & 1U) {
		enemy_x[i] = 240U;
		enemy_dir[i] = RIGHT;
	} else {
		enemy_x[i] = 176U;
		enemy_dir[i] = LEFT;
	}
}

void killEnemy(UBYTE i) {
	enemy_type[i] = ENEMY_NONE;
}

void main() {
	UBYTE tmp;

	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = B8(11100100);
	OBP1_REG = B8(11010000);

	set_bkg_data(0, background_data_length, background_data);
	set_bkg_data(background_data_length, window_data_length, window_data);
	set_bkg_tiles(0, 0, background_tiles_width, background_tiles_height, background_tiles);
	set_win_tiles(0, 0, window_tiles_width, window_tiles_height, window_tiles);

	move_bkg(0U, 112U);
	move_win(7U, 72U);

	set_sprite_data(0, sprites_data_length, sprites_data);

	SHOW_BKG;
	SHOW_SPRITES;
	SHOW_WIN;
	DISPLAY_ON;
	enable_interrupts();

	initIngame();

	while(1) {
		time++;

		if((time & 31U) == 31U) {
			scrollx++;
		}
		if(player_y < SCRLMGN) scrolly = 0;
		else if(player_y > SCRLBTM) scrolly = 112U;
		else scrolly = player_y - SCRLMGN;

		tmp = 112U - scrolly;
		move_bkg(scrollx, 112U - (tmp >> 1U) - (tmp >> 2U));

		if(player_y > SCRLBTM) move_win(7U, 72U);
		else move_win(7U, (72U+SCRLBTM)-player_y);

		updateEnemies();
		updateInput();
		updatePlayer();

		wait_vbl_done();
	}
}
