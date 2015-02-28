#ifndef SELECT_H
#define SELECT_H

void initSelect();
void setTile(UBYTE x, UBYTE y, UBYTE *tile);
void selectTransitionIn();
void selectTransitionOut();
void updateSelectScreen();
void scrollCircles();
void enterSelect();

#endif
