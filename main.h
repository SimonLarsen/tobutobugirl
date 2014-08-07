#ifndef __MAIN_H
#define __MAIN_H

struct Player {
	UBYTE x, y;
	UBYTE xdir, ydir, yspeed;
	UBYTE jumped;
};

struct Enemy {
	UBYTE type;
	UBYTE x, y, dir;
	UBYTE state, frame;
};

void initIngame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void updateEnemies();
void spawnEnemy();
void killEnemy(struct Enemy *e);

#endif
