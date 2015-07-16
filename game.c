#include <gb/gb.h>
#include <rand.h>
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "game.h"
#include "cos.h"
#include "ram.h"
#include "highscore.h"

// Maps
#include "data/bg/hud.h"
#include "data/bg/clock.h"
#include "data/bg/background1.h"
#include "data/bg/background2.h"
#include "data/bg/background3.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE first_load;
UBYTE paused, ingame_state;

UBYTE scrolly, scrolled;
UBYTE next_spawn, last_spawn_x, last_spawn_type;

UBYTE timer, blips, progress, portal_spawned;
UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_bounce;
UBYTE dashing, dashes, dash_xdir, dash_ydir;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];
UBYTE entity_frame;

const UBYTE scrolled_length[4] = { 0U, 16U, 24U, 32U };

const UBYTE entity_sprites[] = {
	0,		// E_NONE
	 // Hazards
	6*4,	// E_SPIKES
	16*4, 	// E_FIREBALL
	 // Enemies
	8*4,	// E_BIRD
	10*4,	// E_BAT
	12*4,	// E_GHOST
	14*4,	// E_ALIEN
	// Powerups
	27*4,	// E_BLIP
	// Special
	21*4,	// E_PORTAL
	28*4,	// E_CLOUD
};

const UBYTE spawn_levels[3][3][8] = {
	// Plains
	{
		{E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT, E_BAT, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BIRD, E_BAT, E_BAT}
	},
	// Clouds
	{
		{E_SPIKES, E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_ALIEN, E_ALIEN, E_BIRD, E_BIRD, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_BAT, E_BAT}
	},
	// Space
	{
		{E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_BAT, E_BAT},
		{E_FIREBALL, E_FIREBALL, E_GHOST, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_BIRD},
		{E_FIREBALL, E_FIREBALL, E_FIREBALL, E_FIREBALL, E_GHOST, E_GHOST, E_GHOST, E_GHOST}
	}
};

void initGame() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0x40U; // 01010000
	BGP_REG = 0xE4U; // 11100100

	// Load tile data
	set_bkg_data(hud_offset, hud_data_length, hud_data);
	set_bkg_data(clock_offset, clock_data_length, clock_data);
	set_win_tiles(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);
	set_sprite_data(0U, sprites_data_length, sprites_data);

	if(first_load) {
		first_load = 0U;

		if(level == 1U) {
			set_bkg_data_rle(background1_offset, background1_data_length, background1_data);
			set_bkg_tiles_rle(0U, 0U, background1_tiles_width, background1_tiles_height, background1_tiles);
		} else if(level == 2U) {
			set_bkg_data_rle(background2_offset, background2_data_length, background2_data);
			set_bkg_tiles_rle(0U, 0U, background2_tiles_width, background2_tiles_height, background2_tiles);
		} else if(level == 3U) {
			set_bkg_data_rle(background3_offset, background3_data_length, background3_data);
			set_bkg_tiles_rle(0U, 0U, background3_tiles_width, background3_tiles_height, background3_tiles);
		}
	}

	clearSprites();
	clearEntities();

	// Init variables
	player_x = 96U;
	player_y = SCRLMGN;
	player_xdir = RIGHT;
	player_ydir = DOWN;
	player_yspeed = 0U;
	player_bounce = 0U;
	dashing = 0U;
	dashes = 3U;
	dash_xdir = 0U;
	dash_ydir = 0U;
	paused = 0U;
	scrolled = 0U;
	scrolly = 0U;

	ingame_state = INGAME_ACTIVE;
	blips = 0U;
	kills = 0U;

	entity_frame = 0U;
	ticks = 0U;
	next_spawn = 0U;
	progress = 0U;
	portal_spawned = 0U;

	timer = 0U;
	remaining_time = 64U;
	elapsed_time = 0U;

	move_bkg(0U, 112U);
	move_win(151U, 0U);

	updateHUDTime();

	SHOW_BKG;
	SHOW_WIN;
	SHOW_SPRITES;
	DISPLAY_ON;

	enable_interrupts();
}

void loadBackground() {
}

void updateInput() {
	updateJoystate();

	if(CLICKED(J_START)) {
		paused = paused ^ 1U;
		if(paused) {
			BGP_REG = 0xF9U; // 11111001
		} else {
			BGP_REG = 0xE4U; // 11100100
		}
	}

	if(paused) return;

	if(ISDOWN(J_LEFT) && !dashing) {
		player_x -= MOVE_SPEED;
		player_xdir = LEFT;
	}

	if(ISDOWN(J_RIGHT) && !dashing) {
		player_x += MOVE_SPEED;
		player_xdir = RIGHT;
	}

	if(CLICKED(KEY_DASH)) {
		if(dashes > 0U) {
			dash_xdir = dash_ydir = NONE;
			if(ISDOWN(J_LEFT)) dash_xdir = LEFT;
			else if(ISDOWN(J_RIGHT)) dash_xdir = RIGHT;
			if(ISDOWN(J_UP)) dash_ydir = UP;
			else if(ISDOWN(J_DOWN)) dash_ydir = DOWN;

			if(dash_xdir != NONE || dash_ydir != NONE) {
				dashing = DASH_TIME;
				dashes--;
				spawnEntity(E_CLOUD, player_x, player_y-6U, 0U);
			}
		}
	}
}

void updatePlayer() {
	UBYTE i, frame, palette, type;

	// Check entity collisions
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] != E_NONE && entity_type[i] <= LAST_COLLIDABLE
		&& player_y > entity_y[i]-12U && player_y < entity_y[i]+11U
		&& player_x > entity_x[i]-12U && player_x < entity_x[i]+12U) {
			type = entity_type[i];
			if(type == E_BLIP) {
				entity_type[i] = E_NONE;
				blips += 16U;
				if(blips > 128U) blips = 128U;
			} else if(type == E_PORTAL
			&& player_y > entity_y[i]-4U && player_y < entity_y[i]+4U
			&& player_x > entity_x[i]-4U && player_x < entity_x[i]+4U) {
				ingame_state = INGAME_COMPLETED;
				player_x = entity_x[i];
				player_y = entity_y[i];
			} else if(type <= E_FIREBALL) {
				killPlayer();
			} else if(type <= LAST_ENEMY) {
				if(player_ydir == DOWN && player_y < entity_y[i]-2U) {
					if(dashing) {
						spawnEntity(E_BLIP, player_x-20U, player_y-8U, 0U);
						spawnEntity(E_BLIP, player_x+20U, player_y-8U, 0U);
						entity_type[i] = E_NONE;
						spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
						kills++;
					}
					else if(type == E_GHOST) {
						entity_type[i] = E_NONE;
						spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
					}
					bounce();
					player_yspeed = JUMP_SPEED;
				} else {
					killPlayer();
				}
			}
		}
	}

	// Dashing
	if(dashing) {
		dashing--;
		player_yspeed = 0U;
		player_ydir = DOWN;
		if(dash_xdir == LEFT) {
			player_x -= DASH_SPEED;
		}
		else if(dash_xdir == RIGHT) {
			player_x += DASH_SPEED;
		}
		if(dash_ydir == UP) {
			player_y -= DASH_SPEED-1U;
			player_yspeed = 7U;
			player_ydir = UP;
		}
		else if(dash_ydir == DOWN) {
			player_y += DASH_SPEED;
		}

		if(!ISDOWN(KEY_DASH)) {
			dashing = 0U;
		}
	}

	// Using jetpack
	if(ISDOWN(KEY_USE) && blips) {
		blips--;
		if(player_ydir == UP && player_yspeed < MAX_FLY_SPEED) {
			player_yspeed += 2U;
		}
		else {
			if(player_yspeed <= 2U) {
				player_yspeed = 2U;
				player_ydir = UP;
			} else {
				player_yspeed -= 2U;
			}
		}
		if(CLICKED(KEY_USE) || (ticks & 7U) == 7U) {
			spawnEntity(E_CLOUD, player_x, player_y+4U, 0U);
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

	// Left and right borders
	if(player_x < 24U) player_x = 24U;
	else if(player_x > 152U) player_x = 152U;

	// Check bounds
	if(player_y > SCREENHEIGHT) {
		killPlayer();
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

	if(dashing) {
		frame = 16U;
	}
	else if(ISDOWN(KEY_USE) && blips && (ticks & 8U)) {
		frame = 12U;
	}

	// Blink
	if(!dashes && (ticks & 4U)) palette = OBJ_PAL1;
	else palette = OBJ_PAL0;

	if(player_xdir == LEFT) {
		setSprite(player_x-16U, player_y, frame, palette);
		setSprite(player_x-8U, player_y, frame+2U, palette);
	} else {
		setSprite(player_x-8U, player_y, frame, FLIP_X | palette);
		setSprite(player_x-16U, player_y, frame+2U, FLIP_X | palette);
	}

	// Update scroll
	scrolly = 0U;
	if(player_y < SCRLMGN) {
		scrolly = SCRLMGN - player_y;
		player_y = SCRLMGN;
	}
}

void updateHUD() {
	UBYTE progressbar;

	// Blips
	setSprite(168U-(blips >> 3), 136U, 104U, OBJ_PAL0);
	setSprite(176U-(blips >> 3), 136U, 106U, OBJ_PAL0);

	// Progress bar
	progressbar = 118U - (progress << 1U) / 3U;
	setSprite(152U, progressbar, 20U, OBJ_PAL0);
	setSprite(160U, progressbar, 22U, OBJ_PAL0);
}

void updateHUDTime() {
	UBYTE index;

	index = (remaining_time+4U) >> 3;
	index = index << 2;

	set_win_tiles(0U, 1U, 2U, 2U, &clock_tiles[index]);
}

void bounce() {
	player_ydir = UP;
	player_bounce = 16U;
	dashes = 3U;
	dashing = 0;
}

void killPlayer() {
	ingame_state = INGAME_DEAD;
}

void updateEntities() {
	UBYTE i, frame, type;
	UBYTE xdist, ydist;

	if((ticks & 7U) == 7U) entity_frame++;

	for(i = 0U; i != MAX_ENTITIES; ++i) {
		type = entity_type[i];

		// Update entity
		switch(type) {
			case E_NONE: continue;

			case E_BLIP:
				if(player_x < entity_x[i]) xdist = entity_x[i] - player_x;
				else xdist = player_x - entity_x[i];
				if(player_y < entity_y[i]) ydist = entity_y[i] - player_y;
				else ydist = player_y - entity_y[i];

				if(xdist < 24U && ydist < 24U) {
					entity_dir[i] = RIGHT;
				}

				if(entity_dir[i] == RIGHT) {
					if(xdist > 3U) {
						if(player_x < entity_x[i]) entity_x[i] -= 2U;
						else entity_x[i] += 2U;
					}
					if(ydist > 3U) {
						if(player_y < entity_y[i]) entity_y[i] -= 2U;
						else entity_y[i] += 2U;
					}
				}
				break;

			case E_BIRD:
				if(ticks & 1U && ingame_state == INGAME_ACTIVE) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] == 152U) entity_dir[i] = LEFT;
					}
					else {
						entity_x[i]--;
						if(entity_x[i] == 24U) entity_dir[i] = RIGHT;
					}
				}
				break;

			case E_ALIEN:
			case E_GHOST:
				if(ticks & 1U && ingame_state == INGAME_ACTIVE) {
					entity_x[i] -= cos32_64[ticks & 63U];
					entity_x[i] += cos32_64[(ticks+1U) & 63U];
					if(ticks & 32U) {
						entity_dir[i] = LEFT;
					} else {
						entity_dir[i] = RIGHT;
					}
				}
				break;

			case E_CLOUD:
				if((ticks & 3U) == 3U) entity_dir[i]++;
				if(entity_dir[i] == 4U) {
					entity_type[i] = E_NONE;
					entity_y[i] = 0U;
					continue;
				}
				break;

			case E_FIREBALL:
				if(ticks & 1U && ingame_state == INGAME_ACTIVE) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] == 152U) entity_dir[i] = LEFT;
					}
					else {
						entity_x[i]--;
						if(entity_x[i] == 24U) entity_dir[i] = RIGHT;
					}
				}
				break;

			case E_PORTAL:
				if(player_x < entity_x[i]) xdist = entity_x[i] - player_x;
				else xdist = player_x - entity_x[i];
				if(player_y < entity_y[i]) ydist = entity_y[i] - player_y;
				else ydist = player_y - entity_y[i];

				if(xdist < 38U && ydist < 38U) {
					if(xdist > 2U) {
						if(player_x < entity_x[i]) player_x += 2U;
						else player_x -= 2U;
					}

					if(ydist > 2U) {
						if(player_y < entity_y[i]) player_y += 2U;
						else player_y -= 2U;
					}

					player_yspeed = 1U;
					player_ydir = UP;
				}

				if((ticks & 7U) == 7U) {
					if(entity_dir[i] == RIGHT) {
						entity_dir[i] = LEFT;
					} else {
						entity_dir[i] = RIGHT;
					}
				}

				break;
		}

		// Scroll entitites
		entity_y[i] += scrolly;
		if(entity_y[i] > SCREENHEIGHT+16U) {
			entity_type[i] = E_NONE;
			continue;
		}

		// Draw entities on screen
		frame = entity_sprites[type];

		switch(type) {
			case E_BLIP:
				frame += (entity_frame & 1U) << 1U;
				setSprite(entity_x[i]-12U, entity_y[i], frame, OBJ_PAL0);
				break;

			case E_CLOUD:
				frame += entity_dir[i] << 2U;
				setSprite(entity_x[i]-16U, entity_y[i], frame, OBJ_PAL0);
				setSprite(entity_x[i]-8U, entity_y[i], frame+2U, OBJ_PAL0);
				break;

			case E_PORTAL:
				if(entity_dir[i] == LEFT) {
					setSprite(entity_x[i]-16U, entity_y[i], frame, OBJ_PAL0);
					setSprite(entity_x[i]-8U, entity_y[i], frame+2U, OBJ_PAL0);
				} else {
					setSprite(entity_x[i]-8U, entity_y[i], frame, OBJ_PAL0 | FLIP_X);
					setSprite(entity_x[i]-16U, entity_y[i], frame+2U, OBJ_PAL0 | FLIP_X);
				}
				break;

			default:
				if(entity_frame & 1U) frame += 4U;
				if(entity_dir[i] == LEFT) {
					setSprite(entity_x[i]-16U, entity_y[i], frame, OBJ_PAL0);
					setSprite(entity_x[i]-8U, entity_y[i], frame+2U, OBJ_PAL0);
				} else {
					setSprite(entity_x[i]-8U, entity_y[i], frame, OBJ_PAL0 | FLIP_X);
					setSprite(entity_x[i]-16U, entity_y[i], frame+2U, OBJ_PAL0 | FLIP_X);
				}
				break;
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

void clearEntities() {
	UBYTE i;
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		entity_type[i] = E_NONE;
	}
}

void initSpawns() {
	UBYTE i, x, y;

	last_spawn_x = 64;
	x = last_spawn_x + 32U;
	y = 112U;
	spawnEntity(E_BAT, x, y, NONE);

	for(i = 0U; i != 3U; ++i) {
		last_spawn_x = (last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
		x = last_spawn_x + 24U;
		y -= 36U;
		spawnEntity(E_BAT, x, y, RIGHT);
	}
	last_spawn_type = E_BAT;
}

void updateSpawns() {
	UBYTE x, dice, type, step;
	next_spawn += scrolly;

	if(next_spawn < 36U) return;

	if(progress < 111U) {
		next_spawn -= 36U;

		last_spawn_x = (last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
		x = last_spawn_x + 24U;

		step = progress / 39U; // TODO: Optimize?
		dice = (UBYTE)rand() & 7U;

		while(dice != 8U) {
			type = spawn_levels[level-1][step][dice];
			switch(type) {
				case E_FIREBALL:
					if(last_spawn_type != E_SPIKES && last_spawn_type != E_FIREBALL) {
						spawnEntity(E_FIREBALL, x, 1U, NONE);
						last_spawn_type = E_FIREBALL;
						dice = 8U;
						break;
					}
					dice++;
					break;
				case E_SPIKES:
					if(last_spawn_type != E_SPIKES && last_spawn_type != E_FIREBALL) {
						spawnEntity(E_SPIKES, x, 1U, NONE);
						last_spawn_type = E_SPIKES;
						dice = 8U;
						break;
					}
					dice++;
					break;
				case E_GHOST:
					spawnEntity(E_GHOST, x, 1U, NONE);
					last_spawn_type = E_GHOST;
					dice = 8U;
					break;
				case E_ALIEN:
					spawnEntity(E_ALIEN, x, 1U, LEFT);
					last_spawn_type = E_ALIEN;
					dice = 8U;
					break;
				case E_BIRD:
					if(x < 80U) {
						spawnEntity(E_BIRD, x, 1U, RIGHT);
					} else {
						spawnEntity(E_BIRD, x, 1U, LEFT);
					}
					last_spawn_type = E_BIRD;
					dice = 8U;
					break;
				case E_BAT:
					spawnEntity(E_BAT, x, 1U, NONE);
					last_spawn_type = E_BAT;
					dice = 8U;
					break;
			}
		}
	}
	else if(progress == 112U && !portal_spawned) {
		spawnEntity(E_PORTAL, 96U, 1U, NONE);
		next_spawn = 0U;
		portal_spawned = 1U;
	}
}

void introAnimation() {
	UBYTE frame;
	for(ticks = 0U; ticks != 64U; ++ticks) {
		frame = 96U - ((ticks >> 4) << 2);
		if(ticks & 8U) {
			setSprite(player_x-16U, player_y, frame, OBJ_PAL0);
			setSprite(player_x-8U, player_y, frame+2U, OBJ_PAL0);
		} else {
			setSprite(player_x-8U, player_y, frame, FLIP_X | OBJ_PAL0);
			setSprite(player_x-16U, player_y, frame+2U, FLIP_X | OBJ_PAL0);
		}

		updateEntities();

		clearRemainingSprites();
		wait_vbl_done();
	}

	for(ticks = 0U; ticks != 32U; ++ticks) {
		if(ticks & 4U) {
			BGP_REG = 0xE4U; // 11100100
		} else {

			BGP_REG = 0x1BU; // 00011011
		}

		setSprite(player_x-8U, player_y, 0U, FLIP_X | OBJ_PAL0);
		setSprite(player_x-16U, player_y, 2U, FLIP_X | OBJ_PAL0);

		updateEntities();

		clearRemainingSprites();
		wait_vbl_done();
	}
}

void intoPortalAnimation() {
	UBYTE frame;

	for(ticks = 0U; ticks != 32U; ++ticks) {
		if(ticks & 4U) {
			BGP_REG = 0xE4U; // 11100100
		} else {
			BGP_REG = 0x1BU; // 00011011
		}

		setSprite(player_x-16U, player_y, 84U, OBJ_PAL0);
		setSprite(player_x-8U, player_y, 86U, OBJ_PAL0);

		clearRemainingSprites();
		wait_vbl_done();
	}

	for(ticks = 0U; ticks != 64U; ++ticks) {
		frame = 84U + ((ticks >> 4) << 2);
		if(ticks & 8U) {
			setSprite(player_x-16U, player_y, frame, OBJ_PAL0);
			setSprite(player_x-8U, player_y, frame+2U, OBJ_PAL0);
		} else {
			setSprite(player_x-8U, player_y, frame, FLIP_X | OBJ_PAL0);
			setSprite(player_x-16U, player_y, frame+2U, FLIP_X | OBJ_PAL0);
		}

		clearRemainingSprites();
		wait_vbl_done();
	}
}

void deathAnimation() {
	UBYTE offset, frame;
	scrolly = 0U;
	for(ticks = 0U; ticks != 48U; ++ticks) {
		if(ticks < 16U) {
			setSprite(player_x-16U, player_y, 72U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 74U, OBJ_PAL0);
		} else if(ticks < 20U) {
			setSprite(player_x-16U, player_y, 76U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 78U, OBJ_PAL0);
		} else if(ticks < 24U) {
			setSprite(player_x-16U, player_y, 80U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 82U, OBJ_PAL0);
		} else {
			offset = ((ticks-16U) >> 1);
			frame = 108U + ((ticks & 4U) >> 1);
			setSprite(player_x-8U-offset, player_y-offset, frame, OBJ_PAL0);
			setSprite(player_x-8U+offset, player_y-offset, frame, OBJ_PAL0);
			setSprite(player_x-8U-offset, player_y+offset, frame, OBJ_PAL0);
			setSprite(player_x-8U+offset, player_y+offset, frame, OBJ_PAL0);
		}

		updateEntities();
		updateHUD();

		clearRemainingSprites();
		wait_vbl_done();
	}
}

void enterGame() {
	first_load = 1U;
ingame_start:
	initGame();
	initSpawns();

	fadeFromWhite(10U);

	introAnimation();

	while(ingame_state == INGAME_ACTIVE) {
		updateInput();
		if(paused) {
			if(CLICKED(J_SELECT)) {
				ingame_state = INGAME_QUIT;
			}
		} else {
			ticks++;
			timer++;

			// Update timing
			if(timer == 60U) {
				timer = 0U;
				elapsed_time++;
				remaining_time--;
				updateHUDTime();

				if(remaining_time == 0U) {
					ingame_state = INGAME_DEAD;
				}
			}

			updatePlayer();
			updateHUD();

			updateEntities();
			updateSpawns();

			// Scroll screen
			scrolled += scrolly;
			if(scrolled > scrolled_length[level]) {
				scrolled -= scrolled_length[level];
				if(progress < 112U) progress++;
				move_bkg(0U, 112U-progress);
			}

			clearRemainingSprites();
		}

		wait_vbl_done();
	}

	if(ingame_state == INGAME_DEAD) {
		deathAnimation();
	}
	else if(ingame_state == INGAME_COMPLETED) {
		intoPortalAnimation();
		addScore(elapsed_time, remaining_time + 2U*kills);
		gamestate = GAMESTATE_WINSCREEN;
	}
	else if(ingame_state == INGAME_QUIT) {
		gamestate = GAMESTATE_SELECT;
	}

	HIDE_SPRITES;
	fadeToWhite(10U);

	if(gamestate == GAMESTATE_INGAME) goto ingame_start;
}

void addScore(UBYTE elapsed_time, UBYTE score) {
	UBYTE i, j;
	UBYTE *data;

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	data = &ram_data[(level - 1U) << 4];
	for(i = 0U; i != 5U; ++i) {
		if(score > data[(i << 1) + 1U]
		|| (score == data[(i << 1) + 1U] && elapsed_time < data[i << 1])) {
			break;
		}
	}
	
	if(i < 5U) {
		for(j = 4U; j != i; --j) {
			data[j << 1] = data[(j - 1U) << 1];
			data[(j << 1) + 1U] = data[((j - 1U) << 1) + 1U];
		}
		data[i << 1] = elapsed_time;
		data[(i << 1) + 1U] = score;
	}

	DISABLE_RAM_MBC1;
}

