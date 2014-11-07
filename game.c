#include <gb/gb.h>
#include <rand.h>
#include "binconst.h"
#include "defines.h"
#include "fade.h"
#include "input.h"
#include "gamestate.h"

#include "game.h"

// Maps
#include "data/bg/background.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE time, loop, dead;
UBYTE next_sprite, sprites_used;
UBYTE scrolly, scrolled, last_spawn_x;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_jumped, player_bounce;

UBYTE cloud_x, cloud_y, cloud_frame;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];
UBYTE entity_frame;

const UBYTE entity_sprites[] = {
	0,		// E_NONE
	 // Hazards
	4*4,	// E_SPIKES
	 // Enemies
	6*4,	// E_BIRD
	8*4,	// E_BAT
	10*4,	// E_GHOST
	// Fruits
	25*4,	// E_GRAPES
	26*4,	// E_PEACH
	// Special
	27*4,	// E_CLOUD
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
};

const UBYTE numbers[] = { 0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U };

void initGame() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = B8(11100100);
	OBP1_REG = B8(11010000);
	BGP_REG = B8(11100100);

	set_bkg_data(0U, background_data_length, background_data);
	set_bkg_tiles(0U, 0U, background_tiles_width, background_tiles_height, background_tiles);

	set_sprite_data(0U, sprites_data_length, sprites_data);

	clearSprites();
	clearEntities();

	spawnEntity(E_BAT, 80U, 110U, NONE);
	spawnEntity(E_BAT, 120U, 70U, NONE);
	spawnEntity(E_BAT, 40U, 30U, NONE);

	player_x = 80U;
	player_y = 60U;
	player_xdir = RIGHT;
	player_ydir = DOWN;
	player_yspeed = 0U;
	player_jumped = 0U;
	player_bounce = 0U;
	dead = 0U;

	cloud_frame = 5U;
	entity_frame = 0U;

	SHOW_BKG;
	HIDE_WIN;
	SHOW_SPRITES;
	DISPLAY_ON;
	enable_interrupts();
}

void updateInput() {
	updateJoystate();

	if(ISDOWN(J_LEFT)) {
		player_x -= MOVE_SPEED;
		player_xdir = LEFT;
	}
	if(ISDOWN(J_RIGHT)) {
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
		if(entity_type[i] != E_NONE && entity_type[i] <= LAST_COLLIDABLE
		&& player_y > entity_y[i]-16U && player_y < entity_y[i]+11U
		&& player_x > entity_x[i]-14U && player_x < entity_x[i]+14U) {
			// Hazards
			if(entity_type[i] == E_SPIKES) {
				killPlayer();
			// Enemies
			} else if(entity_type[i] <= LAST_ENEMY) {
				// Stomp
				if(player_ydir == DOWN && player_y < entity_y[i]-4U) {
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
			}
		}
	}

	// Check bounds
	if(player_y > SCREENH) {
		killPlayer();
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
		setSprite(player_x, player_y, frame, OBJ_PAL1);
		setSprite(player_x+8U, player_y, frame+2U, OBJ_PAL1);
	} else {
		setSprite(player_x+8U, player_y, frame, FLIP_X | OBJ_PAL1);
		setSprite(player_x, player_y, frame+2U, FLIP_X | OBJ_PAL1);
	}

	// Update scroll
	scrolly = 0U;
	if(player_y < SCRLMGN) {
		scrolly = SCRLMGN - player_y;
		player_y = SCRLMGN;
	}

	// Update cloud
	if(cloud_frame != 5U) {
		frame = entity_sprites[E_CLOUD] + (cloud_frame << 2U);

		cloud_y += scrolly;
		setSprite(cloud_x, cloud_y, frame, 0U);
		setSprite(cloud_x+8U, cloud_y, frame+2U, 0U);

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

	for(i = 0U; i != MAX_ENTITIES; ++i) {
		type = entity_type[i];

		// Update entity
		switch(type) {
			case E_NONE: continue;

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
		}

		// Scroll entitites
		entity_y[i] += scrolly;

		if(entity_y[i] > 176U) {
			entity_type[i] = E_NONE;
			entity_y[i] = 0U;
			continue;
		}

		// Draw entities on screen
		frame = entity_sprites[type];
		if(type < FIRST_FRUIT && entity_frame & 1U) frame += 4U;
			
		if(entity_dir[i] == LEFT) {
			setSprite(entity_x[i], entity_y[i], frame, entity_palette[type]);
			setSprite(entity_x[i]+8U, entity_y[i], frame+2U, entity_palette[type]);
		} else {
			setSprite(entity_x[i]+8U, entity_y[i], frame, entity_palette[type] | FLIP_X);
			setSprite(entity_x[i], entity_y[i], frame+2U, entity_palette[type] | FLIP_X);
		}
	}
}

void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir) {
	UBYTE i;
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] == E_NONE) break;
	}
	if(i == MAX_ENTITIES) return;

	entity_x[i] = x;
	entity_y[i] = y;
	entity_type[i] = type;
	entity_dir[i] = dir;
}

void killEntity(UBYTE i) {
	if(entity_type[i] == E_GHOST) {
		if(entity_dir[i] == LEFT) {
			entity_x[i] -= 32U;
		} else {
			entity_x[i] += 32U;
		}
	}

	entity_type[i] = E_NONE;
	entity_x[i] = 168U;
	entity_y[i] = 0U;
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

void clearEntities() {
	UBYTE i;
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		entity_type[i] = E_NONE;
	}
}

void enterGame() {
	UBYTE x;
	initGame();

	loop = 1U;
	next_sprite = 0U;
	time = 0U;
	scrolled = 0U;
	last_spawn_x = 0U;

	move_bkg(0U, 112U);

	while(loop && !dead) {
		time++;

		sprites_used = 0U;

		updatePlayer();

		scrolled += scrolly;
		if(scrolled > 32U) {
			scrolled -= 32U;
			x = (last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
			last_spawn_x = x;
			spawnEntity(E_BAT, 16U+x, 1U, NONE);
		}

		updateEntities();
		updateInput();

		clearRemainingSprites();

		wait_vbl_done();

		scroll_bkg(0U, -scrolly);
	}

	HIDE_SPRITES;
	fadeToWhite();
}
