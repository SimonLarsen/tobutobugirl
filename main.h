#ifndef __MAIN_H
#define __MAIN_H

void initIngame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void updateEnemies();
void spawnEnemy(UBYTE i, UBYTE x, UBYTE y, UBYTE type, UBYTE dir);
void killEnemy(UBYTE i);

const UBYTE enemy_sprites[] = {
	0U,		// ENEMY_NONE
	24U,	// ENEMY_JUMP
	32U,	// ENEMY_SPIKES
	40U,	// ENEMY_BIRD
};

#endif
