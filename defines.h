#ifndef __DEFINES_H
#define __DEFINES_H

#define UP		0U
#define RIGHT	1U
#define DOWN	2U
#define LEFT	3U

#define SCRLMGN 42U
#define SCRLBTM (256U-144U+SCRLMGN)

#define MOVE_SPEED 2U
#define JUMP_SPEED 28U
#define DJUMP_SPEED 25U
#define MAX_YSPEED 24U
#define JUMP_THRESHOLD 10U

#define NUM_WATER 5U
#define NUM_LOW 3U
#define NUM_HIGH 3U
#define NUM_ENEMIES (NUM_WATER+NUM_LOW+NUM_HIGH)

#define WATER_Y 234U
#define LOW_Y 	176U
#define HIGH_Y	120U

#define E_NONE 		0U
#define E_SEAL		1U
#define E_SPIKESEAL	2U
#define E_BIRD		3U
#define E_BALL		4U
#define E_SPIKES	5U

#define SPR_PLAYER	0U
#define SPR_CLOUD	2U
#define SPR_WATER_E	4U
#define SPR_LOW_E	14U
#define SPR_HIGH_E	20U
#define SPR_ENEMIES SPR_WATER_E

#endif
