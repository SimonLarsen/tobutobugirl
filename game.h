#ifndef GAME_H
#define GAME_H

void initGame();
void restoreGame();
UBYTE *getSkinData();
void updateInput();
void updatePlayer();
void updateHUD();
void updateHUDTime();
void killPlayer();
void bouncePlayer(UBYTE entity, UBYTE str);
void updateEntities();
UBYTE spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void clearEntities();
void initSpawns();
void updateSpawns();
void enterGame();
void introAnimation();
void intoPortalAnimation();
void deathAnimation();
void addScore();

#define SCRLMGN 84U

#define NO_LAST_SPAWN 255U

#define MOVE_SPEED        2U
#define DASH_SPEED        4U
#define DASH_TIME        14U
#define SHORT_DASH_TIME   8U
#define BUMP_SPEED       28U
#define ALIEN_BUMP_SPEED 19U
#define MAX_YSPEED       24U
#define MAX_FLY_SPEED    24U

#define KEY_DASH J_A
#define KEY_USE  J_B

#define SPAWN_INTERVAL 36U
#define CLOCK_BONUS 8U

// Entities
#define E_NONE 		0U

#define E_SPIKES   1U
#define E_FIREBALL 2U
#define E_ALIEN    3U
#define E_BAT      4U
#define E_BIRD     5U
#define E_GHOST    6U
#define E_CLOCK    7U
#define E_PORTAL   8U
#define E_CLOUD	   9U

#define FIRST_ENEMY E_SPIKES
#define LAST_ENEMY E_GHOST

#define LAST_COLLIDABLE E_PORTAL

#define MAX_ENTITIES 10U

// States
#define INGAME_ACTIVE    0U
#define INGAME_DEAD      1U
#define INGAME_COMPLETED 2U
#define INGAME_QUIT      3U

#endif
