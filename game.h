#ifndef GAME_H
#define GAME_H

void initGame();
void gameIntro();
void deathAnimation();
void updateInput();
void updatePlayer();
void bouncePlayer();
void killPlayer();
void setCloud(UBYTE x, UBYTE y);
void updateEnemies(UBYTE move);
void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void killEntity(UBYTE i);
void updateScroll();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void enterGame();

#define SCRLMGN 42U
#define SCRLBTM (256U-144U+SCRLMGN)

#define MOVE_SPEED 2U
#define JUMP_SPEED 28U
#define DJUMP_SPEED 25U
#define MAX_YSPEED 24U
#define JUMP_THRESHOLD 10U

#endif
