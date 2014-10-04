#ifndef __DEFINES_H
#define __DEFINES_H

#define NONE	0U

#define UP		0U
#define RIGHT	1U
#define DOWN	2U
#define LEFT	3U

#define NUM_LEVELS 16U

#define E_NONE 		0U

#define E_SPIKES	1U

#define E_SEAL		2U
#define E_BIRD		3U
#define E_BAT		4U

#define E_GRAPES	5U
#define E_PEACH		6U

#define E_CLOUD		7U
#define E_DOOR		8U
#define E_DOOR_OPEN	9U

#define MAX_ENTITIES 15U

#define FIRST_HAZARD E_SPIKES
#define LAST_HAZARD E_SPIKES

#define FIRST_ENEMY E_SEAL
#define LAST_ENEMY E_BAT

#define FIRST_FRUIT E_GRAPES
#define LAST_FRUIT E_PEACH

#define FLIP_X 32U
#define FLIP_Y 64U

#define OBJ_PAL0 0U
#define OBJ_PAL1 16U

#endif
