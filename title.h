#ifndef TITLE_H
#define TITLE_H

void initTitle();
void updateTitleEnemies();
void drawTitleSprites(UBYTE triggered);
void saveMinigameTime();
void enterTitle();

#define TITLE_SCROLL   0U
#define TITLE_MOVE     1U
#define TITLE_MINIGAME 2U
#define TITLE_DEAD     3U

#endif
