#ifndef GAME_H
#define GAME_H

void gameIntro();
void initGame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void setCloud(UBYTE x, UBYTE y);
void updateEnemies(UBYTE move);
void spawnEntity(UBYTE i, UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void killEntity(UBYTE i);
void updateScroll();
void enterGame();

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

#define SPR_PLAYER	0U
#define SPR_CLOUD	2U
#define SPR_WATER_E	4U
#define SPR_LOW_E	14U
#define SPR_HIGH_E	20U
#define SPR_ENEMIES SPR_WATER_E

#endif
