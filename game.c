#include <gb/gb.h>
#include <rand.h>
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "game.h"
#include "cos.h"
#include "ram.h"
#include "highscore.h"
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "pause.h"

// Maps
#include "data/bg/hud.h"
#include "data/bg/clock.h"
// Sprites
#include "data/sprite/sprites.h"
#include "data/sprite/portal.h"
#include "data/sprite/skin1.h"
#include "data/sprite/skin2.h"

UBYTE first_load;
UBYTE scrolled;
UBYTE last_spawn_x, last_spawn_index;
UBYTE next_spawn, next_clock;

UBYTE timer, progress, portal_spawned, repeat_spikes;
UBYTE blips, blip_bar;
UBYTE player_xdir, player_ydir;
UBYTE player_yspeed, player_bounce;
UBYTE dashing, dashes, dash_xdir, dash_ydir;

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];

extern UBYTE plains_song_data;
extern UBYTE clouds_song_data;
extern UBYTE space_song_data;

const UBYTE scrolled_length[4] = {
	16U, // 16 * 111 / 36 = 49
	24U, // 24 * 111 / 36 = 74
	32U, // 32 * 111 / 36 = 98
	40U  // 40 * 111 / 36 = 123
};

const UBYTE clock_interval[4] = {
	13U, //  49/13 = 3
	19U, //  74/19 = 3
	20U, //  98/20 = 4
	25U  // 123/25 = 4
};

const UBYTE allowed_spikes[4] = { 1U, 1U, 1U, 3U };

const UBYTE entity_sprites[] = {
	0,		// E_NONE
	 // Hazards
	9*4,	// E_SPIKES
	19*4, 	// E_FIREBALL
	 // Enemies
	11*4,	// E_BIRD
	13*4,	// E_BAT
	15*4,	// E_GHOST
	17*4,	// E_ALIEN
	// Powerups
	22*4,	// E_CLOCK
	// Special
	21*4,	// E_PORTAL
	28*4	// E_CLOUD
};

const UBYTE spawn_levels[4][3][8] = {
	{ // Plains
		{E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT, E_BAT, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BIRD, E_BAT, E_BAT}
	},
	{ // Clouds
		{E_SPIKES, E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_ALIEN, E_ALIEN, E_BIRD, E_BIRD, E_BAT, E_BAT},
		{E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_BAT, E_BAT}
	},
	{ // Space
		{E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_BAT, E_BAT},
		{E_FIREBALL, E_FIREBALL, E_GHOST, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_BIRD},
		{E_FIREBALL, E_FIREBALL, E_FIREBALL, E_FIREBALL, E_GHOST, E_GHOST, E_GHOST, E_GHOST}
	},
	{ // Dream
		{E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_GHOST, E_BAT, E_BIRD, E_BIRD},
		{E_FIREBALL, E_FIREBALL, E_SPIKES, E_GHOST, E_GHOST, E_GHOST, E_BIRD, E_BIRD},
		{E_FIREBALL, E_FIREBALL, E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_BIRD, E_BIRD}
	}
};

void initGame() {
	UBYTE *skin_data;

	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x16;

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0x40U; // 01010000
	BGP_REG = 0xE4U;  // 11100100

	// Load tile data
	set_bkg_data(hud_offset, hud_data_length, hud_data);
	set_bkg_data(clock_offset, clock_data_length, clock_data);
	set_win_tiles(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);

	skin_data = getSkinData();
	set_sprite_data(24U, sprites_data_length, sprites_data);
	set_sprite_data(0U, 4U, skin_data);
	set_sprite_data(4U, portal_data_length, portal_data);

	if(first_load) {
		first_load = 0U;
		last_progress = 0U;

		setIngameBackground(level);

		setMusicBank(5U);
		if(level == 1U) {
			playMusic(&plains_song_data);
		}
		else if(level == 2U) {
			playMusic(&clouds_song_data);
		}
		else if(level == 3U) {
			playMusic(&space_song_data);
		}
	}
	mus_setPaused(1U);

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
	scroll_y = 0U;

	scene_state = INGAME_ACTIVE;
	blips = 128U;
	blip_bar = 0U;
	kills = 0U;

	ticks = 0U;
	next_spawn = 0U;
	next_clock = clock_interval[level-1U];
	progress = 0U;
	portal_spawned = 0U;
	repeat_spikes = 0U;

	timer = 0U;
	remaining_time = 32U;
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

void restoreGame() {
	disable_interrupts();
	DISPLAY_OFF;

	setIngameBackground(level);

	set_bkg_data(hud_offset, hud_data_length, hud_data);
	set_bkg_data(clock_offset, clock_data_length, clock_data);
	set_win_tiles(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);
	move_bkg(0U, 112U-progress);

	SHOW_BKG;
	SHOW_WIN;
	SHOW_SPRITES;

	DISPLAY_ON;
	enable_interrupts();
}

UBYTE *getSkinData() {
	if(player_skin == 1U) return skin1_data;
	else if(player_skin == 2U) return skin2_data;

	return 0U;
}

void updateInput() {
	updateJoystate();

	if(ISDOWN(J_LEFT) && !dashing) {
		player_x -= MOVE_SPEED;
		player_xdir = LEFT;
	}
	else if(ISDOWN(J_RIGHT) && !dashing) {
		player_x += MOVE_SPEED;
		player_xdir = RIGHT;
	}

	if(CLICKED(KEY_DASH)) {
		if(dashes) {
			dash_xdir = dash_ydir = NONE;
			if(ISDOWN(J_LEFT)) dash_xdir = LEFT;
			else if(ISDOWN(J_RIGHT)) dash_xdir = RIGHT;
			if(ISDOWN(J_UP)) dash_ydir = UP;
			else if(ISDOWN(J_DOWN)) dash_ydir = DOWN;

			if(dash_xdir != NONE || dash_ydir != NONE) {
				dashing = DASH_TIME;
				dashes--;
				spawnEntity(E_CLOUD, player_x, player_y-6U, 0U);
				playSound(SFX_DASH);
			}
		}
	}
}

void updatePlayer() {
	UBYTE i, diff;
	UBYTE frame, palette, type;

	// Check entity collisions
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] != E_NONE && entity_type[i] <= LAST_COLLIDABLE
		&& player_y < entity_y[i]+10U && player_y > entity_y[i]-12U
		&& player_x > entity_x[i]-12U && player_x < entity_x[i]+12U) {
			type = entity_type[i];
			// Spikes
			if(type <= E_FIREBALL) {
				killPlayer();
			// Enemies
			} else if(type <= LAST_ENEMY) {
				if(player_ydir == DOWN && player_y < entity_y[i]+2U) {
					if(dashing && dash_ydir == DOWN) {
						entity_type[i] = E_NONE;
						spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
						blips += 32U;
						if(blips > 128U) blips = 128U;
						kills++;
					}
					else if(type == E_GHOST) {
						entity_type[i] = E_NONE;
						spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
					}
					// Bounce player
					player_ydir = UP;
					player_bounce = 16U;
					dashes = 3U;
					dashing = 0;
					player_yspeed = JUMP_SPEED;
				} else {
					diff = 0U;
					if(player_x < entity_x[i]) diff += entity_x[i] - player_x;
					else diff += player_x - entity_x[i];
					if(player_y < entity_y[i]) diff += entity_y[i] - player_y;
					else diff += player_y - entity_y[i];

					if(diff < 12U) killPlayer();
				}
			// Watch pickup
			} else if(type == E_CLOCK) {
				entity_type[i] = E_NONE;
				remaining_time += CLOCK_BONUS;
				if(remaining_time > 32U) remaining_time = 32;
				updateHUDTime();
			// End level portal
			} else if(type == E_PORTAL
			&& player_y > entity_y[i]-4U && player_y < entity_y[i]+4U
			&& player_x > entity_x[i]-4U && player_x < entity_x[i]+4U) {
				scene_state = INGAME_COMPLETED;
				player_x = entity_x[i];
				player_y = entity_y[i];
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
			if(dashing > DASH_TIME-SHORT_DASH_TIME) {
				dashing -= DASH_TIME - SHORT_DASH_TIME;
			}
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
		if(CLICKED(KEY_USE) || (ticks & 15U) == 15U) {
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
	if(player_x < 23U) player_x = 23U;
	else if(player_x > 153U) player_x = 153U;

	// Check bounds
	if(player_y > SCREENHEIGHT+5U) {
		player_y = SCREENHEIGHT;
		killPlayer();
	}

	// Update sprite
	frame = 0U;
	if(player_bounce) {
		frame = 8U;
		player_bounce--;
	}
	else if(player_ydir == DOWN) {
		frame = 4;
	}

	if(dashing) {
		frame = 20U;
	}
	else if(ISDOWN(KEY_USE) && blips) {
		if(ticks & 8U) frame = 12U;
		else frame = 16U;
	}

	// Blink
	palette = OBJ_PAL0;
	if(!dashes && (ticks & 4U)) palette = OBJ_PAL1;

	// Dash marker
	if(show_dashcounter) {
		setSprite(player_x-12U, player_y-9U, 24U+(dashes << 1), palette);
	}

	if(player_xdir == LEFT) {
		setSprite(player_x-16U, player_y, frame, palette);
		setSprite(player_x-8U, player_y, frame+2U, palette);
	} else {
		setSprite(player_x-8U, player_y, frame, FLIP_X | palette);
		setSprite(player_x-16U, player_y, frame+2U, FLIP_X | palette);
	}

	// Update scroll
	scroll_y = 0U;
	if(player_y < SCRLMGN) {
		scroll_y = SCRLMGN - player_y;
		player_y = SCRLMGN;
	}
}

void updateHUD() {
	UBYTE progressbar;

	if(blip_bar > blips) {
		blip_bar = blips;
	} else if(blip_bar < blips) {
		blip_bar += 2U;
	}
	// Blips
	setSprite(168U-(blip_bar >> 3), 136U, 100U, OBJ_PAL0);
	setSprite(176U-(blip_bar >> 3), 136U, 102U, OBJ_PAL0);

	// Progress bar
	progressbar = 118U - (progress << 1U) / 3U;
	setSprite(152U, progressbar, 104U, OBJ_PAL0);
	setSprite(160U, progressbar, 106U, OBJ_PAL0);

	// Set last progress flag
	if(last_progress) {
		setSprite(153U, 115U-(last_progress >> 1), 96U, OBJ_PAL0);
	}

	// Low on time marker
	if(remaining_time < 16U && ticks & 16U) {
		setSprite(136U, 24U, 32U, OBJ_PAL0);
		setSprite(144U, 24U, 34U, OBJ_PAL0);
	}
}

void updateHUDTime() {
	UBYTE index;

	index = remaining_time >> 2;
	index = index << 2;

	disable_interrupts();
	set_win_tiles(0U, 1U, 2U, 2U, &clock_tiles[index]);
	enable_interrupts();
}

void killPlayer() {
	scene_state = INGAME_DEAD;
}

void updateEntities() {
	UBYTE i, frame, type;
	UBYTE xdist, ydist;

	// Update last spawn position with last spawned
	// enemy if it still exists
	if(last_spawn_index != NO_LAST_SPAWN) {
		if(entity_type[last_spawn_index] != E_NONE) {
			last_spawn_x = entity_x[last_spawn_index]-24U;
		} else {
			last_spawn_index = NO_LAST_SPAWN;
		}
	}

	for(i = 0U; i != MAX_ENTITIES; ++i) {
		type = entity_type[i];

		// Update entity
		switch(type) {
			case E_NONE: continue;

			case E_BIRD:
			case E_FIREBALL:
				if(ticks & 1U && scene_state == INGAME_ACTIVE) {
					if(entity_dir[i] == RIGHT) {
						entity_x[i]++;
						if(entity_x[i] >= 152U) entity_dir[i] = LEFT;
					}
					else {
						entity_x[i]--;
						if(entity_x[i] <= 24U) entity_dir[i] = RIGHT;
					}
				}
				break;

			case E_ALIEN:
			case E_GHOST:
				if(ticks & 1U && scene_state == INGAME_ACTIVE) {
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

			case E_PORTAL:
				if(player_x < entity_x[i]) xdist = entity_x[i] - player_x;
				else xdist = player_x - entity_x[i];
				if(player_y < entity_y[i]) ydist = entity_y[i] - player_y;
				else ydist = player_y - entity_y[i];

				if(xdist < 38U && ydist < 38U) {
					if(xdist > 3U) {
						if(player_x < entity_x[i]) player_x += 3U;
						else player_x -= 3U;
					}

					if(ydist > 3U) {
						if(player_y < entity_y[i]) player_y += 3U;
						else player_y -= 3U;
					}

					player_yspeed = 1U;
					player_ydir = UP;
				}

				if(ticks & 8U) {
					entity_dir[i] = LEFT;
				} else {
					entity_dir[i] = RIGHT;
				}
				break;
		}

		// Scroll entitites
		entity_y[i] += scroll_y;
		if(entity_y[i] > SCREENHEIGHT+16U) {
			entity_type[i] = E_NONE;
			continue;
		}

		// Draw entities on screen
		frame = entity_sprites[type];

		switch(type) {
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
				frame += ((ticks & 8U) >> 1);
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

UBYTE spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir) {
	UBYTE i;
	for(i = 0U; i != MAX_ENTITIES; ++i) {
		if(entity_type[i] == E_NONE) break;
	}
	if(i == MAX_ENTITIES) return NO_LAST_SPAWN;

	entity_x[i] = x;
	entity_y[i] = y;
	entity_type[i] = type;
	entity_dir[i] = dir;

	return i;
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
		last_spawn_index = spawnEntity(E_BAT, x, y, RIGHT);
	}
}

void updateSpawns() {
	UBYTE x, dice, type, step;
	next_spawn += scroll_y;

	if(next_spawn < SPAWN_INTERVAL) return;

	if(progress < 111U) {
		next_spawn -= 36U;

		x = ((last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U) + 24U;

		step = progress / 39U; // TODO: Optimize?
		dice = (UBYTE)rand() & 7U;

		while(dice != 8U) {
			type = spawn_levels[level-1U][step][dice];
			switch(type) {
				case E_FIREBALL:
					if(repeat_spikes < allowed_spikes[level-1U]) {
						last_spawn_index = spawnEntity(E_FIREBALL, x, 1U, NONE);
						repeat_spikes++;
						dice = 8U;
						break;
					}
					dice++;
					break;
				case E_SPIKES:
					if(repeat_spikes < allowed_spikes[level-1U]) {
						last_spawn_index = spawnEntity(E_SPIKES, x, 1U, NONE);
						repeat_spikes++;
						dice = 8U;
						break;
					}
					dice++;
					break;
				case E_GHOST:
					if(x < 40U) x = 40U;
					if(x > 136U) x = 136U;
					last_spawn_index = spawnEntity(E_GHOST, x, 1U, NONE);
					repeat_spikes = 0U;
					dice = 8U;
					break;
				case E_ALIEN:
					if(x < 40U) x = 40U;
					if(x > 136U) x = 136U;
					last_spawn_index = spawnEntity(E_ALIEN, x, 1U, LEFT);
					repeat_spikes = 0U;
					dice = 8U;
					break;
				case E_BIRD:
					if(x < last_spawn_x) {
						last_spawn_index = spawnEntity(E_BIRD, x, 1U, LEFT);
					} else {
						last_spawn_index = spawnEntity(E_BIRD, x, 1U, RIGHT);
					}
					repeat_spikes = 0U;
					dice = 8U;
					break;
				case E_BAT:
					last_spawn_index = spawnEntity(E_BAT, x, 1U, NONE);
					repeat_spikes = 0U;
					dice = 8U;
					break;
			}
		}

		last_spawn_x = x - 24U;

		next_clock--;
		if(next_clock == 0U) {
			next_clock = clock_interval[level-1U];
			x = ((last_spawn_x + 32U + ((UBYTE)rand() & 63U)) & 127U) + 24U;
			spawnEntity(E_CLOCK, x, 1U, NONE);
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
	UBYTE *skin_data;

	for(ticks = 0U; ticks != 64U; ++ticks) {
		frame = 20U - ((ticks >> 4) << 2);
		if(ticks & 8U) {
			setSprite(player_x-16U, player_y, frame, OBJ_PAL0);
			setSprite(player_x-8U, player_y, frame+2U, OBJ_PAL0);
		} else {
			setSprite(player_x-8U, player_y, frame, FLIP_X | OBJ_PAL0);
			setSprite(player_x-16U, player_y, frame+2U, FLIP_X | OBJ_PAL0);
		}

		updateEntities();
		updateHUD();

		clearRemainingSprites();
		snd_update();
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
		updateHUD();

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	disable_interrupts();
	skin_data = getSkinData();
	set_sprite_data(0U, 24U, skin_data);
	set_sprite_data(24U, 4U, sprites_data);
	enable_interrupts();
}

void intoPortalAnimation() {
	UBYTE frame;
	disable_interrupts();
	set_sprite_data(0U, portal_data_length, portal_data);
	enable_interrupts();

	for(ticks = 0U; ticks != 32U; ++ticks) {
		if(ticks & 4U) {
			BGP_REG = 0xE4U; // 11100100
		} else {
			BGP_REG = 0x1BU; // 00011011
		}

		setSprite(player_x-16U, player_y, entity_sprites[E_PORTAL], OBJ_PAL0);
		setSprite(player_x-8U, player_y, entity_sprites[E_PORTAL]+2U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	for(ticks = 0U; ticks != 64U; ++ticks) {
		frame = 4U + ((ticks >> 4) << 2);
		if(ticks & 8U) {
			setSprite(player_x-16U, player_y, frame, OBJ_PAL0);
			setSprite(player_x-8U, player_y, frame+2U, OBJ_PAL0);
		} else {
			setSprite(player_x-8U, player_y, frame, FLIP_X | OBJ_PAL0);
			setSprite(player_x-16U, player_y, frame+2U, FLIP_X | OBJ_PAL0);
		}

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}
}

void deathAnimation() {
	UBYTE offset, frame;
	UBYTE *skin_data;
	skin_data = getSkinData();
	set_sprite_data(0U, 8U, skin_data+384UL);
	set_sprite_data(8U, portal_data_length, portal_data);

	scroll_y = 0U;
	for(ticks = 0U; ticks != 48U; ++ticks) {
		if(ticks < 16U) {
			if(player_xdir == LEFT) {
				setSprite(player_x-16U, player_y, 0U, OBJ_PAL0);
				setSprite(player_x-8U, player_y, 2U, OBJ_PAL0);
			} else {
				setSprite(player_x-8U, player_y, 0U, FLIP_X | OBJ_PAL0);
				setSprite(player_x-16U, player_y, 2U, FLIP_X | OBJ_PAL0);
			}
		} else if(ticks < 20U) {
			setSprite(player_x-16U, player_y, 4U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 6U, OBJ_PAL0);
		} else if(ticks < 24U) {
			setSprite(player_x-16U, player_y, 8U, OBJ_PAL0);
			setSprite(player_x-8U, player_y, 10U, OBJ_PAL0);
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
		snd_update();
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

	mus_setPaused(0U);

	while(scene_state == INGAME_ACTIVE) {
		updateInput();
		if(CLICKED(J_START)) {
			scene_state = enterPause();
			restoreGame();
		}

		ticks++;
		timer++;

		// Update timing
		if(timer == 60U) {
			timer = 0U;
			elapsed_time++;
			remaining_time--;
			updateHUDTime();

			if(remaining_time == 0U) {
				scene_state = INGAME_DEAD;
			}
		}

		updatePlayer();
		updateHUD();

		updateEntities();
		updateSpawns();

		// Scroll screen
		scrolled += scroll_y;
		if(scrolled >= scrolled_length[level-1U]) {
			scrolled -= scrolled_length[level-1U];
			if(progress < 112U) progress++;
			move_bkg(0U, 112U-progress);
		}

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}
	stopMusic();

	if(scene_state == INGAME_DEAD) {
		deathAnimation();
		if(progress > last_progress) {
			last_progress = progress;
		}
	}
	else if(scene_state == INGAME_COMPLETED) {
		intoPortalAnimation();
		addScore();
		gamestate = GAMESTATE_WINSCREEN;
	}
	else if(scene_state == INGAME_QUIT) {
		gamestate = GAMESTATE_SELECT;
	}

	HIDE_SPRITES;
	fadeToWhite(10U);

	if(gamestate == GAMESTATE_INGAME) goto ingame_start;
}

void addScore() {
	UBYTE i, j, score;
	UBYTE *data;

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	score = TOTAL_SCORE;

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

		last_highscore_level = level;
		last_highscore_slot = i;
	}

	if(level > levels_completed) {
		levels_completed = level;
		if(level == 1U) {
			unlocked_bits = UNLOCKED_CLOUDS;
		}
		else if(level == 2U) {
			unlocked_bits = UNLOCKED_SPACE | UNLOCKED_MUSIC;
		}
		else if(level == 3U) {
			unlocked_bits = UNLOCKED_DREAM;
		}
	}

	DISABLE_RAM_MBC1;
}
