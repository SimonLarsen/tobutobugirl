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
UBYTE entities, killables;
UBYTE next_sprite, sprites_used;
UBYTE scrolly, scrollx;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_jumped, player_bounce;

UBYTE cloud_x, cloud_y, cloud_frame;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];
UBYTE entity_var1[MAX_ENTITIES];
UBYTE entity_frame;

#define IS_KILLABLE(x) (x != E_NONE && x <= LAST_FRUIT && x != E_SPIKES)

const UBYTE entity_sprites[] = {
	   0,	// E_NONE
	 // Hazards
	 4*4,	// E_SPIKES
	 // Enemies
	 6*4,	// E_SEAL
	 8*4,	// E_BIRD
	10*4,	// E_BAT
	12*4,	// E_GHOST
	// Fruits
	23*4,	// E_GRAPES
	24*4,	// E_PEACH
	// Special
	25*4,	// E_CLOUD
	30*4,	// E_DOOR
	31*4,	// E_DOOR_OPEN
};

const UBYTE entity_palette[] = {
	OBJ_PAL0,	// E_NONE
	// Hazards
	OBJ_PAL1,	// E_SPIKES
	// Enemies
	OBJ_PAL0,	// E_SEAL
	OBJ_PAL1,	// E_BIRD
	OBJ_PAL1,	// E_BAT
	OBJ_PAL1,	// E_GHOST
	// Fruits
	OBJ_PAL1,	// E_GRAPES
	OBJ_PAL1,	// E_PEACH
	// Special
	OBJ_PAL0,	// E_CLOUD
	OBJ_PAL0,	// E_DOOR
	OBJ_PAL0,	// E_DOOR_OPEN
};

void initGame() {
	UBYTE i;

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


	for(i = 0U; i != MAX_ENTITIES; ++i) entity_type[i] = E_NONE;

	killables = 0U;
	entities = 0U;
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		spawnEntity(levels[level][i][0], levels[level][i][1], levels[level][i][2], levels[level][i][3]);
	}

	time = 0U;
	scrollx = 0U;
	scrolly = 112U;

	player_x = 80U;
	player_y = 16U;
	player_xdir = RIGHT;
	player_ydir = DOWN;
	player_yspeed = 0U;
	player_jumped = 0U;
	player_bounce = 0U;
	dead = 0U;

	cloud_frame = 5U;
	entity_frame = 0U;

	SHOW_BKG;
	SHOW_SPRITES;
	SHOW_WIN;
	DISPLAY_ON;
	enable_interrupts();
}

void gameIntro() {
	HIDE_SPRITES;
	fadeFromWhite();
	SHOW_SPRITES;
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
	if(player_x < 8U) player_x = 8U;
	else if(player_x > 152U) player_x = 152U;

	// Check entity collisions
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] != E_NONE
		&& player_y > entity_y[i]-16U && player_y < entity_y[i]+13U
		&& player_x > entity_x[i]-14U && player_x < entity_x[i]+14U) {
			// Hazards
			if(entity_type[i] <= LAST_HAZARD) {
				killPlayer();
			// Enemies
			} else if(entity_type[i] <= LAST_ENEMY) {
				// Stomp
				if(player_ydir == DOWN && player_y < entity_y[i]-8U) {
					player_ydir = UP;
					player_yspeed = JUMP_SPEED;
					player_jumped = 0U;
					player_bounce = 16U;
					killEntity(i);
					setCloud(player_x, player_y+5U);
				}
				// Hit
				else {
					killPlayer();
				}
			} else if(entity_type[i] <= LAST_FRUIT) {
				killEntity(i);
			} else if(entity_type[i] == E_DOOR_OPEN) {
				loop = 0U;
			}
		}
	}

	// Check bounds
	if(player_y > 246U) {
		killPlayer();
	}
	else if(player_y < 4U) {
		player_y = 4U;
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
	if(player_bounce != 0U) {
		frame = 8U;
		player_bounce--;
	}
	else if(player_ydir == DOWN) {
		frame = 4;
	}

	if(player_xdir == LEFT) {
		setSprite(player_x, player_y-scrolly+16U, frame, OBJ_PAL1);
		setSprite(player_x+8U, player_y-scrolly+16U, frame+2U, OBJ_PAL1);
	} else {
		setSprite(player_x+8U, player_y-scrolly+16U, frame, FLIP_X | OBJ_PAL1);
		setSprite(player_x, player_y-scrolly+16U, frame+2U, FLIP_X | OBJ_PAL1);
	}

	// Update cloud
	if(cloud_frame != 5U) {
		frame = entity_sprites[E_CLOUD] + (cloud_frame << 2U);

		setSprite(cloud_x, cloud_y-scrolly+16U, frame, 0U);
		setSprite(cloud_x+8U, cloud_y-scrolly+16U, frame+2U, 0U);

		if((time & 3U) == 3U) cloud_frame++;
	}
}

void killPlayer() {
	dead = 1U;
}

void setCloud(UBYTE x, UBYTE y) {
	cloud_x = x;
	cloud_y = y;
	cloud_frame = 0U;
}

void updateEntities() {
	UBYTE i, frame, type;

	if((time & 7U) == 7U) entity_frame++;

	for(i = 0U; i != entities; ++i) {
		type = entity_type[i];

		switch(type) {
			case E_NONE:
				continue;
			case E_BIRD:
				if(time & 1U) {
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
			case E_DOOR:
				if(killables == 0U) {
					entity_type[i] = E_DOOR_OPEN;
				}
		}

		// Draw entities on screen
		if(entity_y[i]+16U > scrolly && entity_y[i]-16U < scrolly+143U) {
			frame = entity_sprites[type];
			if(type < FIRST_FRUIT && entity_frame & 1U) frame += 4U;
			
			if(entity_dir[i] == LEFT) {
				setSprite(entity_x[i], entity_y[i]-scrolly+16U, frame, entity_palette[type]);
				setSprite(entity_x[i]+8U, entity_y[i]-scrolly+16U, frame+2U, entity_palette[type]);
			} else {
				setSprite(entity_x[i]+8U, entity_y[i]-scrolly+16U, frame, entity_palette[type] | FLIP_X);
				setSprite(entity_x[i], entity_y[i]-scrolly+16U, frame+2U, entity_palette[type] | FLIP_X);
			}
		}
	}
}

void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir) {
	UBYTE i;
	if(type == E_NONE) return;

	i = entities;

	entity_x[i] = x;
	entity_y[i] = y;
	entity_type[i] = type;
	entity_dir[i] = dir;
	entity_var1[i] = 0U;

	if(IS_KILLABLE(type)) killables++;
	entities++;
}

void killEntity(UBYTE i) {
	if(entity_type[i] == E_GHOST) {
		if(entity_dir[i] == LEFT) {
			entity_x[i] -= 32U;
		} else {
			entity_x[i] += 32U;
		}
		entity_var1[i]++;
		if(entity_var1[i] != 3U) return;
	}

	if(IS_KILLABLE(entity_type[i])) {
		killables--;
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
}

void enterGame() {
	initGame();

	//gameIntro();

	loop = 1U;
	next_sprite = 0U;
	while(loop && !dead) {
		time++;

		updateScroll();

		sprites_used = 0U;

		updateEntities();
		updateInput();
		updatePlayer();

		clearRemainingSprites();

		if(CLICKED(J_SELECT)) {
			killables = 1U;
			break;
		}

		wait_vbl_done();
	}

	if (dead) {
		//deathAnimation();
		gamestate = GAMESTATE_GAME;
	}
	else if(killables == 0U) {
		gamestate = GAMESTATE_LEVEL;
		completed[level] = 1U;
	}
	else {
		gamestate = GAMESTATE_LEVEL;
	}

	HIDE_SPRITES;
	fadeToWhite();
}
