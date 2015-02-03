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
#include "data/bg/background.h"
// Sprites
#include "data/sprite/sprites.h"

UBYTE time, paused, dead;
UBYTE blink, flash;
UBYTE blips, powerup, active_powerup, powerup_time, has_shield, progress;
UBYTE next_sprite, sprites_used;
UBYTE next_spawn, last_spawn_x, last_spawn_type, skip_spawns;
UBYTE scrolly, scrolled;

UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_bounce;
UBYTE dashing, dashes, dash_xdir, dash_ydir;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];
UBYTE entity_frame;

#define IS_ENEMY(x) (x >= FIRST_ENEMY && x <= LAST_ENEMY)
#define SET_POWERUP_HUD(x) (set_win_tiles(15U, 0U, 2U, 2U, &powerups_tiles[x << 2U]))

const UBYTE cosx32[64] = {
	0U, 0U, 0U, 1U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 11U, 13U, 14U, 16U, 18U, 19U, 21U, 22U, 24U, 25U, 26U, 27U, 28U, 29U, 30U, 31U, 31U, 32U, 32U, 32U, 32U, 32U, 31U, 31U, 30U, 29U, 28U, 27U, 26U, 25U, 24U, 22U, 21U, 19U, 18U, 16U, 14U, 13U, 11U, 10U, 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 1U, 0U, 0U
};

const UBYTE dash_time[4] = { 0U, 10U, 10U, 10U };

const UBYTE entity_sprites[] = {
	0,		// E_NONE
	 // Hazards
	7*4,	// E_SPIKES
	 // Enemies
	9*4,	// E_BIRD
	11*4,	// E_BAT
	13*4,	// E_GHOST
	15*4,	// E_ALIEN
	// Misc
	21*4,	// E_JUMPPAD
	// Powerups
	94,		// E_PADDLE
	24*4,	// E_BLIP
	// Fruits
	25*4,	// E_GRAPES
	26*4,	// E_PEACH
	// Special
	27*4,	// E_CLOUD
};

void initGame() {
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = B8(11010000);
	OBP1_REG = B8(01010000);
	BGP_REG = B8(11100100);

	set_bkg_data(0U, powerups_data_length, powerups_data);
	set_bkg_data(hud_offset, hud_data_length, hud_data);
	set_bkg_data(background_offset, background_data_length, background_data);

	set_bkg_tiles(0U, 0U, background_tiles_width, background_tiles_height, background_tiles);
	set_win_tiles(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);

	set_sprite_data(0U, sprites_data_length, sprites_data);

	clearSprites();
	clearEntities();

	player_x = 80U;
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
	time = 0U;
	next_spawn = 0U;
	last_spawn_type = E_NONE;
	last_spawn_x = 80U;
	skip_spawns = 0U;
	progress = 0U;

	move_bkg(0U, 112U);
	move_win(7U, 128U);

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
		dashing = dash_time[dashes];
		dashes--;

		dash_xdir = dash_ydir = NONE;
		if(ISDOWN(J_LEFT)) dash_xdir = LEFT;
		else if(ISDOWN(J_RIGHT)) dash_xdir = RIGHT;
		if(ISDOWN(J_UP)) dash_ydir = UP;
		else if(ISDOWN(J_DOWN)) dash_ydir = DOWN;
	}

	if(CLICKED(KEY_USE)) {
		if(blips == 16U) {
			blips = 0U;
		}
		else if(powerup != 0U) {
			switch(powerup) {
				case P_PADDLE:
					for(i = 0U; i != MAX_ENTITIES; ++i) {
						if(entity_type[i] == E_PADDLE) {
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
					break;
				case P_ROCKET:
					active_powerup = P_ROCKET;
					powerup_time = P_ROCKET_TIME;
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
	UBYTE tmpx, tmpy;

	if((time & 3U) == 3U) {
		powerup_time--;
		if(powerup_time == 0U) active_powerup = 0U;
	}

	// Check entity collisions
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] != E_NONE && entity_type[i] <= LAST_COLLIDABLE
		&& player_y > entity_y[i]-16U && player_y < entity_y[i]+11U
		&& player_x > entity_x[i]-12U && player_x < entity_x[i]+12U) {
			if(active_powerup == P_ROCKET && entity_type[i] != E_JUMPPAD) {
				killEntity(i);
				spawnEntity(E_CLOUD, player_x, player_y-6U, 0U);
			} else if(entity_type[i] == E_SPIKES) {
				killPlayer();
			} else if(entity_type[i] == E_JUMPPAD) { 
				bounce();
				player_yspeed = JUMPPAD_SPEED;
			} else if(entity_type[i] == E_PADDLE) {
				bounce();
				player_yspeed = JUMPPAD_SPEED;
				killEntity(i);
			} else if(entity_type[i] == E_BLIP) {
				killEntity(i);
				blink = 13U;
				if(powerup == 0U) {
					blips += 2U;
				}
			} else if(entity_type[i] <= LAST_ENEMY) {
				if(player_ydir == DOWN && player_y < entity_y[i]-2U) {
					bounce();
					player_yspeed = JUMP_SPEED;
					killEntity(i);
					spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
				} else if(has_shield) {
					bounce();
					player_yspeed = SHIELD_JUMP_SPEED;
					killEntity(i);
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
			player_y -= DASH_SPEED-2U;
			player_yspeed = 14U;
			player_ydir = UP;
		}
		else if(dash_ydir == DOWN) {
			player_y += DASH_SPEED;
		}
	}

	// Apply powerups
	if(active_powerup == P_BALLOON) {
		player_yspeed = 14U;
		player_ydir = UP;
		if(powerup_time > 20U || powerup_time & 1U) {
			setSprite(player_x, player_y-16U, 80U, OBJ_PAL0);
			setSprite(player_x+8U, player_y-16U, 82U, OBJ_PAL0);
		}
	}
	else if(active_powerup == P_ROCKET) {
		player_yspeed = 32U;
		player_ydir = UP;
		if((time & 15U) == 15U) {
			spawnEntity(E_CLOUD, player_x, player_y+2U, 0U);
		}
	}
	if(has_shield) {
		setSprite(player_x, player_y+9U, 76U, OBJ_PAL0);
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
	if(player_x < 8U) player_x = 8U;
	else if(player_x > 152U) player_x = 152U;

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

	// Rocket powerup
	if(active_powerup == P_ROCKET) frame = 16U;
	else if(dashing) frame = 20U;

	// Blink
	if((blink & 2U) == 2U) palette = OBJ_PAL1;
	else palette = OBJ_PAL0;
	if(blink != 0U && (time & 1U)) blink--;

	if(player_xdir == LEFT) {
		setSprite(player_x, player_y, frame, palette);
		setSprite(player_x+8U, player_y, frame+2U, palette);
	} else {
		setSprite(player_x+8U, player_y, frame, FLIP_X | palette);
		setSprite(player_x, player_y, frame+2U, FLIP_X | palette);
	}

	// Draw reticule
	tmpx = player_x;
	tmpy = player_y;
	if(ISDOWN(J_LEFT)) tmpx -= 30U;
	if(ISDOWN(J_RIGHT)) tmpx += 30U;
	if(ISDOWN(J_UP)) tmpy -= 30U;
	if(ISDOWN(J_DOWN)) tmpy += 30U;

	if(tmpx || tmpy) {
		if((time & 8U) == 8U) {
			setSprite(tmpx, tmpy, 104U, OBJ_PAL0);
			setSprite(tmpx+8U, tmpy, 106U, OBJ_PAL0);
		} else {
			setSprite(tmpx+8U, tmpy, 104U, FLIP_X | OBJ_PAL0);
			setSprite(tmpx, tmpy, 106U, FLIP_X | OBJ_PAL0);
		}
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
	if(blips == 16U) {
		if(powerup == 0U) {
			powerup = FIRST_POWERUP;

			SET_POWERUP_HUD(powerup);
		} else {
			if((time & 7U) == 7U) {
				powerup++;
				if(powerup > LAST_POWERUP) {
					powerup = FIRST_POWERUP;
				}
				SET_POWERUP_HUD(powerup);
			}
		}
	} else {
		setSprite(128U, 160U-blips, 92U, OBJ_PAL0);
		setSprite(136U, 160U-blips, 92U, OBJ_PAL0);
	}

	progressbar = (progress << 1U) / 3U;
	setSprite(24U+progressbar, 145U, 24U, OBJ_PAL0);
	setSprite(32U+progressbar, 145U, 26U, OBJ_PAL0);
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

	if((time & 7U) == 7U) entity_frame++;

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

				if(xdist < 24U && ydist < 24U && xdist+ydist < 30U) {
					entity_dir[i] = RIGHT;
				}

				if(entity_dir[i] == RIGHT) {
					if(xdist > 2U) {
						if(player_x < entity_x[i]) entity_x[i] -= 2U;
						else entity_x[i] += 2U;
					}
					if(ydist > 2U) {
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
				if(time & 1U) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] == 144U) entity_dir[i] = LEFT;
					}
					else {
						entity_x[i]--;
						if(entity_x[i] == 16U) entity_dir[i] = RIGHT;
					}
				}
				break;

			case E_ALIEN:
				if(time & 1U) {
					entity_x[i] -= cosx32[time & 63U];
					entity_x[i] += cosx32[(time+1U) & 63U];
				}
				break;

			case E_CLOUD:
				if((time & 3U) == 3U) entity_dir[i]++;
				if(entity_dir[i] == 5U) {
					entity_type[i] = E_NONE;
					entity_y[i] = 0U;
					continue;
				}
		}

		// Scroll entitites
		entity_y[i] += scrolly;
		if(entity_y[i] > 136U && entity_y[i] < 200U) {
			entity_type[i] = E_NONE;
			entity_y[i] = 0U;
			continue;
		}

		// Draw entities on screen
		frame = entity_sprites[type];

		switch(type) {
			case E_BLIP:
				frame += (entity_frame & 1U) << 1U;
				setSprite(entity_x[i]+4U, entity_y[i], frame, OBJ_PAL0);
				break;

			case E_PADDLE:
				setSprite(entity_x[i], entity_y[i], frame, OBJ_PAL0);
				setSprite(entity_x[i]+8U, entity_y[i], frame, OBJ_PAL0);
				break;

			case E_CLOUD:
				frame += entity_dir[i] << 2U;
				setSprite(entity_x[i], entity_y[i], frame, OBJ_PAL0);
				setSprite(entity_x[i]+8U, entity_y[i], frame+2U, OBJ_PAL0);
				break;
				
			default:
				if(type < FIRST_FRUIT && entity_frame & 1U) frame += 4U;
					
				if(entity_dir[i] == LEFT) {
					setSprite(entity_x[i], entity_y[i], frame, OBJ_PAL0);
					setSprite(entity_x[i]+8U, entity_y[i], frame+2U, OBJ_PAL0);
				} else {
					setSprite(entity_x[i]+8U, entity_y[i], frame, OBJ_PAL0 | FLIP_X);
					setSprite(entity_x[i], entity_y[i], frame+2U, OBJ_PAL0 | FLIP_X);
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

void initSpawns() {
	spawnEntity(E_BAT, 32U, 1U, NONE);
	spawnEntity(E_BAT, 128U, 1U, NONE);

	spawnEntity(E_JUMPPAD, 80U, 78U, NONE);
}

void updateSpawns() {
	UBYTE x, y, type;
	next_spawn += scrolly;
	if(next_spawn > 36U) {
		next_spawn -= 36U;

		if(skip_spawns != 0) {
			skip_spawns--;
		} else if(active_powerup == P_ROCKET && powerup_time < 6U) {
			last_spawn_x = (player_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
			x = last_spawn_x + 16U;

			spawnEntity(E_JUMPPAD, x, 1U, NONE);
			skip_spawns = 1U;
			last_spawn_type = E_JUMPPAD;
		} else {
			last_spawn_x = (last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U;
			x = last_spawn_x + 16U;

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
				case 2: // E_ALIEN
					spawnEntity(E_ALIEN, x, 1U, LEFT);
					last_spawn_type = E_ALIEN;
					break;
				case 3: // E_JUMPPAD
					if(last_spawn_type != E_SPIKES) {
						spawnEntity(E_JUMPPAD, x, 1U, NONE);
						skip_spawns = 2U;
						last_spawn_type = E_JUMPPAD;
						break;
					}
				case 4: // E_SPIKES
					if(last_spawn_type != E_SPIKES && last_spawn_type != E_JUMPPAD) {
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
		x = 16U + ((UBYTE)rand() & 127U);
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
			time++;

			// Flash background
			if(flash != 0) {
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
