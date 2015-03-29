#ifndef GAME_H
#define GAME_H

void saveScore();
void initGame();
void updateInput();
void updatePlayer();
void updateHUD();
void updateHUDTime();
void killPlayer();
void updateEntities();
void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void bounce();
void clearEntities();
void initSpawns();
void updateSpawns();
void enterGame();
void deadAnimation();

#define MAPW 10U
#define MAPH 16U

#define SCRLMGN 84U

#define MOVE_SPEED			2U
#define DASH_SPEED			4U
#define DASH_TIME			14U
#define JUMP_SPEED			29U
#define SHIELD_JUMP_SPEED	33U
#define DJUMP_SPEED			26U
#define JUMPPAD_SPEED		39U
#define MAX_YSPEED			24U

#define RETICULE_MAX_OFFSET	30U

#define P_BALLOON_TIME	100U
#define P_ROCKET_TIME	25U
#define P_SHIELD_TIME	100U

#define KEY_DASH	J_A
#define KEY_USE		J_B

// Entities
#define E_NONE 		0U

#define E_SPIKES	1U
#define E_FIREBALL	2U
#define E_BIRD		3U
#define E_BAT		4U
#define E_GHOST		5U
#define E_ALIEN		6U
#define E_JUMPPAD	7U
#define E_PADDLE	8U
#define E_BLIP		9U
#define E_PORTAL	10U
#define E_CLOUD		11U

#define FIRST_ENEMY E_SPIKES
#define LAST_ENEMY E_ALIEN

#define LAST_COLLIDABLE E_PORTAL

#define MAX_ENTITIES 18U

// Powerups
#define	P_NONE		0U
#define P_PADDLE	1U
#define P_BALLOON	2U
#define P_BATS		3U
#define P_ROCKET	4U
#define P_SHIELD	5U

#define FIRST_POWERUP	P_PADDLE
#define LAST_POWERUP	P_SHIELD

// States
#define INGAME_ACTIVE		0U
#define INGAME_DEAD			1U
#define INGAME_COMPLETED	2U
#define INGAME_QUIT			3U

#endif
