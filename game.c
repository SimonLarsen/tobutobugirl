#include <gb/gb.h>
#include <rand.h>
#include "binconst.h"
#include "defines.h"
#include "fade.h"
#include "input.h"
#include "gamestate.h"
#include "game.h"

// Maps
#include "data/bg/powerups.h"
#include "data/bg/hud.h"
#include "data/bg/clock.h"
#include "data/bg/background.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE ticks, paused, dead;
UBYTE next_sprite, sprites_used;
UBYTE blink, flash;
UBYTE blips, powerup, active_powerup, powerup_time, has_shield, progress;

UBYTE scrolly, scrolled;
UBYTE next_spawn, last_spawn_x, last_spawn_type, skip_spawns;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_bounce;
UBYTE dashing, dashes, dash_xdir, dash_ydir;
UBYTE timer, remaining_time, elapsed_seconds, elapsed_minutes;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];
UBYTE entity_frame;

#define IS_ENEMY(x) ((x) >= FIRST_ENEMY && (x) <= LAST_ENEMY)
#define SET_POWERUP_HUD(x) (set_win_tiles(16U, 0U, 2U, 2U, &powerups_tiles[(x) << 2]))

const UBYTE cosx32[64] = {
	0U, 0U, 0U, 1U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 11U, 13U, 14U, 16U,
	18U, 19U, 21U, 22U, 24U, 25U, 26U, 27U, 28U, 29U, 30U, 31U, 31U, 32U, 32U,
	32U, 32U, 32U, 31U, 31U, 30U, 29U, 28U, 27U, 26U, 25U, 24U, 22U, 21U, 19U,
	18U, 16U, 14U, 13U, 11U, 10U, 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 1U, 0U, 0U
};

const UBYTE entity_sprites[] = {
	0,		// E_NONE
	 // Hazards
	7*4,	// E_SPIKES
	24*4, 	// E_FIREBALL
	 // Enemies
	9*4,	// E_BIRD
	11*4,	// E_BAT
	13*4,	// E_GHOST
	15*4,	// E_ALIEN
	// Misc
	21*4,	// E_JUMPPAD
	// Powerups
	106,	// E_PADDLE
	27*4,	// E_BLIP
	// Fruits
	25*4,	// E_GRAPES
	26*4,	// E_PEACH
	// Special
	28*4,	// E_CLOUD
};

void initGame() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = B8(11010000);
	OBP1_REG = B8(01010000);
	BGP_REG = B8(11100100);

	// Load tile data
	set_bkg_data(0U, powerups_data_length, powerups_data);
	set_bkg_data(hud_offset, hud_data_length, hud_data);
	set_bkg_data(clock_offset, clock_data_length, clock_data);
	set_bkg_data(background_offset, background_data_length, background_data);

	set_bkg_tiles(0U, 0U, background_tiles_width, background_tiles_height, background_tiles);
	set_win_tiles(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);

	set_sprite_data(0U, sprites_data_length, sprites_data);

	clearSprites();
	clearEntities();

	// Init variables
	player_x = 96U;
	player_y = 40U;
	player_xdir = RIGHT;
	player_ydir = DOWN;
	player_yspeed = 0U;
	player_bounce = 0U;
	dashing = 0U;
	dashes = 3U;
	dash_xdir, dash_ydir = 0U;

	dead = 0U;
	blink = 0U;
	flash = 0U;
	blips = 0U;
	powerup = 0U;
	active_powerup = 0U;
	has_shield = 0U;

	entity_frame = 0U;
	next_sprite = 0U;
	ticks = 0U;
	next_spawn = 0U;
	last_spawn_type = E_NONE;
	last_spawn_x = 96U;
	skip_spawns = 0U;
	progress = 0U;

	timer = 0U;
	remaining_time = 64U;
	elapsed_seconds = 0U;
	elapsed_minutes = 0U;

	move_bkg(0U, 112U);
	move_win(7U, 128U);

	// Clear HUD
	SET_POWERUP_HUD(0U);
	updateHUDTime();

	SHOW_BKG;
	SHOW_WIN;
	SHOW_SPRITES;
	DISPLAY_ON;

	enable_interrupts();
}

void updateInput() {
	UBYTE i;
	updateJoystate();

	if(CLICKED(J_START)) {
		paused = paused ^ 1U;
		BGP_REG = B8(11111001);
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

	if(CLICKED(KEY_DASH) && dashes && !active_powerup) {
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

	if(CLICKED(KEY_USE)) {
		if(blips >= 32U) {
			blips = 0U;
		}
		else if(powerup != 0U) {
			switch(powerup) {
				case P_PADDLE:
					for(i = 0U; i != MAX_ENTITIES; ++i) {
						if(entity_type[i] == E_PADDLE) {
							entity_type[i] = E_NONE;
							break;
						}
					}
					spawnEntity(E_PADDLE, 80U, 136U, RIGHT);
					break;
				case P_BATS:
					for(i = 0U; i != MAX_ENTITIES; ++i) {
						if(IS_ENEMY(entity_type[i])) {
							entity_type[i] = E_BAT;
						}
					}
					flash = 6U;
					break;
				case P_BALLOON:
					active_powerup = P_BALLOON;
					powerup_time = P_BALLOON_TIME;
					dashes = 3U;
					break;
				case P_ROCKET:
					active_powerup = P_ROCKET;
					powerup_time = P_ROCKET_TIME;
					dashes = 3U;
					break;
				case P_SHIELD:
					has_shield = 1U;
					break;
			}
			powerup = 0U;
			SET_POWERUP_HUD(powerup);
		}
	}
}

void updatePlayer() {
	UBYTE i, frame, palette;

	if((ticks & 3U) == 3U) {
		powerup_time--;
		if(powerup_time == 0U) active_powerup = 0U;
	}

	// Check entity collisions
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] != E_NONE && entity_type[i] <= LAST_COLLIDABLE
		&& player_y > entity_y[i]-14U && player_y < entity_y[i]+11U
		&& player_x > entity_x[i]-12U && player_x < entity_x[i]+12U) {
			if(entity_type[i] == E_BLIP) {
				entity_type[i] = E_NONE;
				if(powerup == 0U) {
					blips += 2U;
				}
			} else if(active_powerup == P_ROCKET) {
				entity_type[i] = E_NONE;
				spawnEntity(E_CLOUD, player_x, player_y-6U, 0U);
			} else if(entity_type[i] <= E_FIREBALL) {
				killPlayer();
			} else if(entity_type[i] == E_PADDLE) {
				bounce();
				player_yspeed = JUMPPAD_SPEED;
				entity_type[i] = E_NONE;
			} else if(entity_type[i] <= LAST_ENEMY) {
				if(player_ydir == DOWN && player_y < entity_y[i]-2U) {
					if(dashing) {
						spawnEntity(E_BLIP, player_x-16U, player_y-8U, 0U);
						spawnEntity(E_BLIP, player_x+16U, player_y-8U, 0U);
						entity_type[i] = E_NONE;
						spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
					}
					bounce();
					player_yspeed = JUMP_SPEED;
				} else if(has_shield) {
					entity_type[i] = E_NONE;
					has_shield = 0U;
					spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
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
	// Apply powerups
	else if(active_powerup == P_BALLOON) {
		player_yspeed = 14U;
		player_ydir = UP;
		if(powerup_time > 20U || powerup_time & 1U) {
			setSprite(player_x-16U, player_y-16U, 100U, OBJ_PAL0);
			setSprite(player_x-8U, player_y-16U, 102U, OBJ_PAL0);
		}
	}
	else if(active_powerup == P_ROCKET) {
		player_yspeed = 32U;
		player_ydir = UP;
		if((ticks & 31U) == 31U) {
			spawnEntity(E_CLOUD, player_x, player_y+2U, 0U);
		}
	}

	// Draw shield
	if(has_shield) {
		setSprite(player_x-16U, player_y+9U, 76U, OBJ_PAL0);
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
	else if(player_x > 168U) player_x = 168U;

	// Check bounds
	if(player_y > SCREENHEIGHT-12U) {
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

	if(active_powerup == P_ROCKET) frame = 16U;
	else if(dashing) frame = 20U;

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
	if(blips >= 32U) {
		if(powerup == 0U) {
			powerup = FIRST_POWERUP;
			SET_POWERUP_HUD(powerup);
		} else {
			if((ticks & 7U) == 7U) {
				powerup++;
				if(powerup > LAST_POWERUP) {
					powerup = FIRST_POWERUP;
				}
				SET_POWERUP_HUD(powerup);
			}
		}
	} else {
		setSprite(120U, 160U-(blips >> 1), 104U, OBJ_PAL0);
		setSprite(128U, 160U-(blips >> 1), 104U, OBJ_PAL0);
	}

	progressbar = (progress << 1U) / 3U;
	setSprite(40U+progressbar, 145U, 24U, OBJ_PAL0);
	setSprite(48U+progressbar, 145U, 26U, OBJ_PAL0);
}

void updateHUDTime() {
	UBYTE index;

	index = (remaining_time+4U) >> 3;
	index = index << 2;

	set_win_tiles(2U, 0U, 2U, 2U, &clock_tiles[index]);
}

void bounce() {
	player_ydir = UP;
	player_bounce = 16U;
	dashes = 3U;
	dashing = 0;
}

void killPlayer() {
	dead = 1U;
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
				if(active_powerup == P_ROCKET) break;

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

			case E_PADDLE:
				entity_y[i] = 136U-scrolly;
				if(player_x < entity_x[i]) entity_x[i]--;
				else if(player_x > entity_x[i]) entity_x[i]++;
				break;

			case E_BIRD:
				if(ticks & 1U) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] == 160U) entity_dir[i] = LEFT;
					}
					else {
						entity_x[i]--;
						if(entity_x[i] == 32U) entity_dir[i] = RIGHT;
					}
				}
				break;

			case E_GHOST:
				if(ticks & 1U) {
					entity_x[i] -= cosx32[ticks & 63U];
					entity_x[i] += cosx32[(ticks+1U) & 63U];
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

			case E_ALIEN:
				if((ticks & 63U) == 63U) {
					frame = UPLEFT;
					xdist = entity_x[i]-8U;
					if(player_y > entity_y[i]) frame = DOWNLEFT;
					if(player_x > entity_x[i]) {
						frame++;
						xdist += 16U;
					}
					spawnEntity(E_FIREBALL, xdist, entity_y[i]+4U, frame);
				}
				break;

			case E_FIREBALL:
				switch(entity_dir[i]) {
					case UPLEFT:
						entity_x[i]--;
						entity_y[i]--;
						break;
					case UPRIGHT:
						entity_x[i]++;
						entity_y[i]--;
						break;
					case DOWNLEFT:
						entity_x[i]--;
						entity_y[i]++;
						break;
					case DOWNRIGHT:
						entity_x[i]++;
						entity_y[i]++;
						break;
				}
				break;
		}

		// Scroll entitites
		entity_y[i] += scrolly;
		if(entity_y[i] > 136U || entity_x[i] > 196U) {
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

			case E_FIREBALL:
				setSprite(entity_x[i]-12U, entity_y[i], frame, OBJ_PAL0);
				break;

			case E_PADDLE:
				setSprite(entity_x[i]-16U, entity_y[i], frame, OBJ_PAL0);
				setSprite(entity_x[i]-8U, entity_y[i], frame, OBJ_PAL0);
				break;

			case E_CLOUD:
				frame += entity_dir[i] << 2U;
				setSprite(entity_x[i]-16U, entity_y[i], frame, OBJ_PAL0);
				setSprite(entity_x[i]-8U, entity_y[i], frame+2U, OBJ_PAL0);
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

void initSpawns() {
	spawnEntity(E_BAT, 96U, 74U, NONE);
	spawnEntity(E_BAT, 48U, 38U, NONE);
	spawnEntity(E_BAT, 144U, 2U, NONE);
	last_spawn_x = 144U;
}


void updateSpawns() {
	UBYTE x, y, type;
	next_spawn += scrolly;
	if(next_spawn > 36U) {
		next_spawn -= 36U;

		if(skip_spawns != 0) {
			skip_spawns--;
		} else {
			if(active_powerup == P_ROCKET && powerup_time < 5U) return;

			last_spawn_x = (last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
			x = last_spawn_x + 32U;

			type = (UBYTE)rand() & 7U;
			switch(type) {
				case 0: // E_BIRD
				case 1:
					if(x < 80U) {
						spawnEntity(E_BIRD, x, 1U, RIGHT);
					} else {
						spawnEntity(E_BIRD, x, 1U, LEFT);
					}
					last_spawn_type = E_BIRD;
					break;
				case 2: // E_GHOST
					spawnEntity(E_GHOST, x, 1U, LEFT);
					last_spawn_type = E_GHOST;
					break;
				case 3: // E_ALIEN
					spawnEntity(E_ALIEN, x, 1U, NONE);
					last_spawn_type == E_ALIEN;
					break;
				case 4: // E_SPIKES
					if(last_spawn_type != E_SPIKES) {
						spawnEntity(E_SPIKES, x, 1U, NONE);
						last_spawn_type = E_SPIKES;
						break;
					}
				default: // E_BAT
					spawnEntity(E_BAT, x, 1U, NONE);
					last_spawn_type = E_BAT;
					break;
			}
		}

		// Spawn blips
		x = 32U + ((UBYTE)rand() & 127U);
		y = 232U + ((UBYTE)rand() & 15U);
		spawnEntity(E_BLIP, x, y, NONE);
	}
}

void enterGame() {
	initGame();
	initSpawns();

	while(!dead) {
		updateInput();
		if(!paused) {
			ticks++;
			timer++;
			// Update timing
			if(timer == 60U) {
				timer = 0U;
				elapsed_seconds++;
				remaining_time--;
				updateHUDTime();

				if(elapsed_seconds == 60U) {
					elapsed_seconds = 0U;
					elapsed_minutes++;
				}
			}

			// Flash background
			if(flash) {
				flash--;
				BGP_REG = B8(00011011);
			} else BGP_REG = B8(11100100);

			sprites_used = 0U;

			updatePlayer();
			updateHUD();

			updateEntities();

			updateSpawns();

			scrolled += scrolly;
			if(scrolled > 32U) {
				scrolled -= 32U;
				progress++;
				move_bkg(0U, 115U-progress);
			}

			clearRemainingSprites();
		}

		wait_vbl_done();
	}

	HIDE_SPRITES;
	fadeToWhite();
}
