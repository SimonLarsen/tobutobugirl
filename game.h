#ifndef GAME_H
#define GAME_H

void initGame();
void updateInput();
void updatePlayer();
void updateHUD();
void killPlayer();
void updateEntities();
void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void bounce();
void killEntity(UBYTE i);
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void clearEntities();
void initSpawns();
void updateSpawns();
void enterGame();

#define MAPW 10U
#define MAPH 16U

#define SCRLMGN 68U

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
#define P_ROCKET_TIME	40U
#define P_SHIELD_TIME	100U

#define KEY_DASH	J_A
#define KEY_USE		J_B

// Entities
#define E_NONE 		0U

#define E_SPIKES	1U

#define E_BIRD		2U
#define E_BAT		3U
#define E_GHOST		4U
#define E_ALIEN		5U

#define E_JUMPPAD	6U

#define E_PADDLE	7U
#define E_BLIP		8U

#define E_GRAPES	9U
#define E_PEACH		10U

#define E_CLOUD		11U

#define FIRST_ENEMY E_SPIKES
#define LAST_ENEMY E_ALIEN

#define FIRST_FRUIT E_GRAPES
#define LAST_FRUIT E_PEACH

#define LAST_COLLIDABLE E_PEACH

#define MAX_ENTITIES 18U

// Entity properties
#define FLIP_X 32U
#define FLIP_Y 64U

#define OBJ_PAL0 0U
#define OBJ_PAL1 16U

// Powerups
#define	P_NONE		0U
#define P_PADDLE	1U
#define P_BALLOON	2U
#define P_BATS		3U
#define P_ROCKET	4U
#define P_SHIELD	5U

#define FIRST_POWERUP	P_PADDLE
#define LAST_POWERUP	P_SHIELD

#endif
