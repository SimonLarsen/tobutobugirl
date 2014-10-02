#ifndef GAME_H
#define GAME_H

void gameIntro();
void deathAnimation();
void initGame();
void updateInput();
void updatePlayer();
void bouncePlayer();
void killPlayer();
void setCloud(UBYTE x, UBYTE y);
void updateEnemies(UBYTE move);
void spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
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

#define SPR_PLAYER	0U
#define SPR_CLOUD	2U
#define SPR_ENEMIES	4U

#define SPR_PLAYER_DEAD 12U

#endif
