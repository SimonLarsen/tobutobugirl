#ifndef GAME_H
#define GAME_H

void initGame();
void updateInput();
void updatePlayer();
void updateHUD();
void killPlayer();
void updateEntities();
void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void killEntity(UBYTE i);
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void clearEntities();
void initSpawns();
void updateSpawns();
void enterGame();

#define MAPW 10U
#define MAPH 16U

#define SCRLMGN 68U

#define MOVE_SPEED 2U
#define JUMP_SPEED 29U
#define DJUMP_SPEED 26U
#define JUMPPAD_SPEED 39U
#define MAX_YSPEED 24U

#endif
