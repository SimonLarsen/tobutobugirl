#ifndef __MAIN_H
#define __MAIN_H

void initIngame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void setCloud(UBYTE x, UBYTE y);
void updateEnemies();
void spawnEntity(UBYTE i, UBYTE x, UBYTE y, UBYTE type, UBYTE dir);
void killEntity(UBYTE i);

const UBYTE entity_sprites[] = {
	0U,		// E_NONE
	24U,	// E_SEAL
	32U,	// E_SHIELD
	40U,	// E_BIRD
	56U,	// E_SPIKES
	64U,	// E_BAT
	72U,	// E_GRAPES
	76U,	// E_BANANA
	80U,	// E_APPLE
	84U,	// E_CLOUD
};

const UBYTE entity_palette[] = {
	0U,	// E_NONE
	0U,	// E_SEAL
	0U,	// E_SHIELD
	1U,	// E_BIRD
	1U,	// E_SPIKES
	0U,	// E_BAT
	0U,	// E_GRAPES
	0U,	// E_BANANA
	0U,	// E_APPLE
	0U, // E_CLOUD
};


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
#define E_SHIELD	2U
#define E_BIRD		3U
#define E_SPIKES	4U
#define E_BAT		5U
#define E_GRAPES	6U
#define E_BANANA	7U
#define E_APPLE		8U
#define E_CLOUD		9U

#define FIRST_FRUIT E_GRAPES

#define SPR_PLAYER	0U
#define SPR_CLOUD	2U
#define SPR_WATER_E	4U
#define SPR_LOW_E	14U
#define SPR_HIGH_E	20U
#define SPR_ENEMIES SPR_WATER_E

#endif
