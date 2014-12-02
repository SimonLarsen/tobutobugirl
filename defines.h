#ifndef __DEFINES_H
#define __DEFINES_H

#define NONE	0U

#define UP		0U
#define RIGHT	1U
#define DOWN	2U
#define LEFT	3U

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
#define P_UMBRELLA	5U

#define FIRST_POWERUP	P_PADDLE
#define LAST_POWERUP	P_ROCKET

#define P_BALLOON_TIME	100U
#define P_ROCKET_TIME	60U

#endif
