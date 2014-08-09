#ifndef __MAIN_H
#define __MAIN_H

void initIngame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void updateEnemy(UBYTE i);
void updateEnemies();
void spawnWaterEnemy();
void spawnLowEnemy();
void spawnHighEnemy();
void killEnemy(UBYTE i);

#endif
