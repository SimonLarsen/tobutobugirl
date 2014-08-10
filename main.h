#ifndef __MAIN_H
#define __MAIN_H

void initIngame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void updateEnemies();
void spawnEntity(UBYTE i, UBYTE x, UBYTE y, UBYTE type, UBYTE dir);
void killEntity(UBYTE i);

const UBYTE entity_sprites[] = {
	0U,		// ENEMY_NONE
	24U,	// ENEMY_JUMP
	32U,	// ENEMY_SPIKES
	40U,	// ENEMY_BIRD
};

#endif
