#include <gb/gb.h>
#include <rand.h>
#include "binconst.h"
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "game.h"
#include "cos.h"

// Maps
#include "data/bg/powerups.h"
#include "data/bg/hud.h"
#include "data/bg/clock.h"
#include "data/bg/background1.h"
#include "data/bg/background2.h"
#include "data/bg/background3.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE paused, ingame_state;
UBYTE blink, flash;
UBYTE blips, powerup, active_powerup, powerup_time, has_shield, progress;

UBYTE scrolly, scrolled;
UBYTE next_spawn, last_spawn_x, last_spawn_type;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_bounce;
UBYTE dashing, dashes, dash_xdir, dash_ydir;
UBYTE timer, remaining_time, elapsed_seconds, elapsed_minutes, portal_spawned;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];
UBYTE entity_frame;

#define IS_ENEMY(x) ((x) >= FIRST_ENEMY && (x) <= LAST_ENEMY)
#define SET_POWERUP_HUD(x) (set_win_tiles(16U, 0U, 2U, 2U, &powerups_tiles[(x) << 2]))

const UBYTE scrolled_length[4] = { 0U, 16U, 24U, 32U };

const UBYTE entity_sprites[] = {
	0,		// E_NONE
	 // Hazards
	7*4,	// E_SPIKES
	17*4, 	// E_FIREBALL
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
	// Special
	22*4,	// E_PORTAL
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
	set_win_tiles(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);
	set_sprite_data(0U, sprites_data_length, sprites_data);

	if(level == 1U) {
		set_bkg_tiles(0U, 0U, background1_tiles_width, background1_tiles_height, background1_tiles);
		set_bkg_data(background1_offset, background1_data_length, background1_data);
	} else if(level == 2U) {
		set_bkg_tiles(0U, 0U, background2_tiles_width, background2_tiles_height, background2_tiles);
		set_bkg_data(background2_offset, background2_data_length, background2_data);
	} else if(level == 3U) {
		set_bkg_tiles(0U, 0U, background3_tiles_width, background3_tiles_height, background3_tiles);
		set_bkg_data(background3_offset, background3_data_length, background3_data);
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

	ingame_state = INGAME_ACTIVE;
	blink = 0U;
	flash = 0U;
	blips = 0U;
	powerup = 0U;
	active_powerup = 0U;
	has_shield = 0U;

	entity_frame = 0U;
	ticks = 0U;
	next_spawn = 0U;
	last_spawn_type = E_NONE;
	last_spawn_x = 96U;
	progress = 0U;
	portal_spawned = 0U;

	timer = 0U;
	remaining_time = 64U;
	elapsed_seconds = 0U;
	elapsed_minutes = 0U;

	move_bkg(0U, 115U);
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
	UBYTE i, frame, palette, type;

	if((ticks & 3U) == 3U) {
		powerup_time--;
		if(powerup_time == 0U) active_powerup = 0U;
	}

	// Check entity collisions
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] != E_NONE && entity_type[i] <= LAST_COLLIDABLE
		&& player_y > entity_y[i]-14U && player_y < entity_y[i]+11U
		&& player_x > entity_x[i]-12U && player_x < entity_x[i]+12U) {
			type = entity_type[i];
			if(type == E_BLIP) {
				entity_type[i] = E_NONE;
				if(powerup == 0U) {
					blips += 2U;
				}
			} else if(type == E_PORTAL) {
				ingame_state = INGAME_COMPLETED;
			} else if(active_powerup == P_ROCKET) {
				entity_type[i] = E_NONE;
				spawnEntity(E_CLOUD, player_x, player_y-6U, 0U);
			} else if(type <= E_FIREBALL) {
				killPlayer();
			} else if(type == E_PADDLE) {
				bounce();
				player_yspeed = JUMPPAD_SPEED;
				entity_type[i] = E_NONE;
			} else if(type <= LAST_ENEMY) {
				if(player_ydir == DOWN && player_y < entity_y[i]-2U) {
					if(dashing) {
						spawnEntity(E_BLIP, player_x-20U, player_y-8U, 0U);
						spawnEntity(E_BLIP, player_x+20U, player_y-8U, 0U);
						entity_type[i] = E_NONE;
						spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
					}
					else if(type == E_GHOST) {
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
		setSprite(player_x-16U, player_y+9U, 98U, OBJ_PAL0);
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
		setSprite(136U, 160U-(blips >> 1), 104U, OBJ_PAL0);
		setSprite(144U, 160U-(blips >> 1), 104U, OBJ_PAL0);
	}

	progressbar = (progress << 1U) / 3U;
	setSprite(43U+progressbar, 145U, 24U, OBJ_PAL0);
	setSprite(51U+progressbar, 145U, 26U, OBJ_PAL0);
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
				if(ticks & 1U && ingame_state == INGAME_ACTIVE) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] == 168U) entity_dir[i] = LEFT;
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
						if(entity_x[i] == 168U) entity_dir[i] = LEFT;
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

				if(xdist < 48U && ydist < 48U) {
					if(player_x < entity_x[i]) player_x += 1U;
					else player_x -= 2U;

					if(player_y < entity_y[i]) player_y += 1U;
					else player_y -= 2U;

					player_yspeed = 0U;
					player_ydir = DOWN;
				}

				break;
		}

		// Scroll entitites
		entity_y[i] += scrolly;
		// Note: Not relevant without blips
		//if((entity_y[i] > 136U && entity_y[i] < 232U)
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

	for(i = 0U; i != 4U; ++i) {
		last_spawn_x = (last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
		x = last_spawn_x + 32U;
		y -= 36U;
		spawnEntity(E_BAT,  x, y, NONE);
	}
}

void updateSpawns() {
	UBYTE x, type;
	next_spawn += scrolly;

	if(next_spawn < 36U) return;

	if(progress < 111U) {
		next_spawn -= 36U;

		if(active_powerup == P_ROCKET && powerup_time < 5U) return;

		last_spawn_x = (last_spawn_x + 16U + ((UBYTE)rand() & 63U)) & 127U;
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
				spawnEntity(E_GHOST, x, 1U, NONE);
				last_spawn_type = E_GHOST;
				break;
			case 3: // E_ALIEN
				spawnEntity(E_ALIEN, x, 1U, LEFT);
				last_spawn_type = E_ALIEN;
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
	else if(progress == 115U && !portal_spawned) {
		spawnEntity(E_PORTAL, 96U, 1U, NONE);
		next_spawn = 0U;
		portal_spawned = 1U;
	}
}

void deathAnimation() {
	UBYTE offset, frame;
	scrolly = 0U;
	for(ticks = 0U; ticks != 48U; ++ticks) {
		if(ticks < 16U) {
			setSprite(player_x-16U, player_y, 76U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 78U, OBJ_PAL0);
		} else if(ticks < 20U) {
			setSprite(player_x-16U, player_y, 80U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 82U, OBJ_PAL0);
		} else if(ticks < 24U) {
			setSprite(player_x-16U, player_y, 84U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 86U, OBJ_PAL0);
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
	initGame();
	initSpawns();

	fadeFromWhite(10U);

	while(ingame_state == INGAME_ACTIVE) {
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

				if(remaining_time == 0U) {
					ingame_state = INGAME_DEAD;
				}
			}

			// Flash background
			if(flash) {
				flash--;
				BGP_REG = B8(00011011);
			} else BGP_REG = B8(11100100);

			updatePlayer();
			updateHUD();

			updateEntities();

			updateSpawns();

			scrolled += scrolly;
			if(scrolled > scrolled_length[level]) {
				scrolled -= scrolled_length[level];
				if(progress < 115U) progress++;
				move_bkg(0U, 115U-progress);
			}

			clearRemainingSprites();
		}

		wait_vbl_done();
	}

	if(ingame_state == INGAME_DEAD) {
		deathAnimation();
	}
	else if(ingame_state == INGAME_COMPLETED) {
		gamestate = GAMESTATE_SELECT;
	}

	HIDE_SPRITES;
	fadeToWhite(10U);
}
