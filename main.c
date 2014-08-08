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

struct Enemy water_enemy[NUM_WATER];
struct Enemy low_enemy[NUM_LOW];
struct Enemy high_enemy[NUM_HIGH];
UBYTE enemy_count[ENEMY_TYPES];

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
	player_y = 72U;
	player_xdir = RIGHT;
	player_ydir = DOWN;
	player_yspeed = 0U;
	player_jumped = 0U;
	player_bounce = 0U;

	for(i = 0U; i < ENEMY_TYPES; ++i) enemy_count[i] = 0U;
	for(i = 0U; i < NUM_WATER; ++i) water_enemy[i].type = ENEMY_NONE;
	for(i = 0U; i < NUM_LOW; ++i) low_enemy[i].type = ENEMY_NONE;
	for(i = 0U; i < NUM_HIGH; ++i) high_enemy[i].type == ENEMY_NONE;

	for(i = SPR_LOW_E; i < SPR_LOW_E+NUM_LOW*2U; ++i) {
		set_sprite_prop(i, B8(00010000));
	}
	for(i = SPR_HIGH_E; i < SPR_HIGH_E+NUM_HIGH*2U; ++i) {
		set_sprite_prop(i, B8(00010000));
	}

	for(i = 0U; i < 3U; ++i) {
		spawnWaterEnemy();
	}

	spawnLowEnemy();
	spawnLowEnemy();
	spawnHighEnemy();
	spawnHighEnemy();
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

	// Check water enemies
	if(player_y >= WATER_Y-14U && player_y <= WATER_Y-6U) {
		for(i = 0U; i < NUM_WATER; ++i) {
			if(water_enemy[i].type != ENEMY_NONE
			&& player_x > water_enemy[i].x-16U && player_x < water_enemy[i].x+16U) {
				bouncePlayer();
				spawnWaterEnemy();
				killEnemy(&water_enemy[i]);
				break;
			}
		}
	}

	// Check low enemies
	if(player_y >= LOW_Y-12U && player_y <= LOW_Y-8U) {
		for(i = 0U; i < NUM_LOW; ++i) {
			if(low_enemy[i].type != ENEMY_NONE
			&& player_x > low_enemy[i].x-16U && player_x < low_enemy[i].x+16U) {
				if(player_ydir == DOWN) {
					bouncePlayer();
					killEnemy(&low_enemy[i]);
					spawnLowEnemy();
				}
				break;
			}
		}
	}

	// Check high enemies
	if(player_y >= HIGH_Y-12U && player_y <= HIGH_Y-8U) {
		for(i = 0U; i < NUM_HIGH; ++i) {
			if(high_enemy[i].type != ENEMY_NONE
			&& player_x > high_enemy[i].x-16U && player_x < high_enemy[i].x+16U) {
				if(player_ydir == DOWN) {
					bouncePlayer();
					killEnemy(&high_enemy[i]);
					spawnHighEnemy();
				}
			}
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

void updateEnemy( struct Enemy *e) {
	UBYTE offset;

	e->frame++;
	switch(e->type) {
		case ENEMY_NONE:
			move_sprite(e->sprite, 0U, 0U);
			move_sprite(e->sprite+1U, 0U, 0U);
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
			if(e->type == ENEMY_JUMP) {
				set_sprite_tile(e->sprite, 24U+offset);
				set_sprite_tile(e->sprite+1U, 26U+offset);
			}
			else if(e->type == ENEMY_SPIKES) {
				set_sprite_tile(e->sprite, 44U+offset);
				set_sprite_tile(e->sprite+1U, 46U+offset);
			}
			move_sprite(e->sprite, e->x, e->y-scrolly+16U);
			move_sprite(e->sprite+1U, e->x+8U, e->y-scrolly+16U);
			break;

		case ENEMY_BIRD:
			if(e->frame & 1U) {
				if(e->state == RIGHT) e->x++;
				else e->x--;
			}
			if((e->frame & 7U) == 7U) {
				e->state = e->state ^ 1U;
			}

			offset = 64U + (e->state << 2U);
			if(e->dir == RIGHT) offset += 8U;
			
			set_sprite_tile(e->sprite, offset);
			set_sprite_tile(e->sprite+1, offset+2U);

			move_sprite(e->sprite, e->x, e->y-scrolly+16U);
			move_sprite(e->sprite+1U, e->x+8U, e->y-scrolly+16U);

			break;
	}
}

void updateEnemies() {
	UBYTE i;

	for(i = 0U; i < NUM_WATER; ++i) {
		updateEnemy(&water_enemy[i]);
	}
	for(i = 0; i < NUM_LOW; ++i) {
		updateEnemy(&low_enemy[i]);
	}
	for(i = 0; i < NUM_HIGH; ++i) {
		updateEnemy(&high_enemy[i]);
	}
}

void spawnWaterEnemy() {
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
	e->sprite = SPR_WATER_E + (i << 1U);
	e->x = 16U + i*32U;
	e->y = WATER_Y;
	e->dir = (UBYTE)rand() % 1U;
	e->state = 0U;
	e->frame = 0U;
}

void spawnLowEnemy() {
	UBYTE i;
	struct Enemy *e;

	for(i = 0U; i < NUM_LOW; ++i) {
		if(low_enemy[i].type == ENEMY_NONE) {
			break;
		}
	}
	if(i == NUM_LOW) return;

	e = &low_enemy[i];
	e->sprite = SPR_LOW_E + (i << 1U);
	e->type = ENEMY_BIRD;
	e->state = 0U;
	e->frame = 0U;
	if((UBYTE)rand() & 1U) {
		e->x = 0U;
		e->dir = RIGHT;
	} else {
		e->x = 160U;
		e->dir = LEFT;
	}
	e->y = LOW_Y;
}

void spawnHighEnemy() {
	UBYTE i;
	struct Enemy *e;

	for(i = 0U; i < NUM_HIGH; ++i) {
		if(high_enemy[i].type == ENEMY_NONE) {
			break;
		}
	}
	if(i == NUM_HIGH) return;

	e = &high_enemy[i];
	e->sprite = SPR_HIGH_E + (i << 1U);
	e->type = ENEMY_BIRD;
	e->state = 0U;
	e->frame = 0U;
	if((UBYTE)rand() & 1U) {
		e->x = 0U;
		e->dir = RIGHT;
	} else {
		e->x = 160U;
		e->dir = LEFT;
	}
	e->y = HIGH_Y;
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

		move_bkg(scrollx, 112U - ((112U - scrolly) >> 1U));

		if(player_y > SCRLBTM) move_win(7U, 72U);
		else move_win(7U, (72U+SCRLBTM)-player_y);

		updateEnemies();
		updateInput();
		updatePlayer();

		wait_vbl_done();
	}
}
