#ifndef __DEFINES_H
#define __DEFINES_H

#define NONE	0U

#define UP		0U
#define RIGHT	1U
#define DOWN	2U
#define LEFT	3U

#define NUM_LEVELS 16U

// Tiles
#define T_WALL	1U
#define T_UNDEF	2U

// Entities
#define E_NONE 		0U

#define E_SPIKES	1U

#define E_SEAL		2U
#define E_BIRD		3U
#define E_BAT		4U
#define E_GHOST		5U

#define E_GRAPES	6U
#define E_PEACH		7U

#define E_CLOUD		8U
#define E_DOOR		9U
#define E_DOOR_OPEN	10U

#define MAX_ENTITIES 20U

#define FIRST_HAZARD E_SPIKES
#define LAST_HAZARD E_SPIKES

#define FIRST_ENEMY E_SEAL
#define LAST_ENEMY E_GHOST

#define FIRST_FRUIT E_GRAPES
#define LAST_FRUIT E_PEACH

// Entity properties
#define FLIP_X 32U
#define FLIP_Y 64U

#define OBJ_PAL0 0U
#define OBJ_PAL1 16U

#endif
