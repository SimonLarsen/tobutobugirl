#ifndef SELECT_H
#define SELECT_H

void initSelect();
UBYTE *selectGetBannerData();
void selectUpdateScreen();
void _selectUpdateScreen();
void selectUpdateSprites();
void enterSelect();

#define SELECT_IDLE    0U
#define SELECT_FADEOUT 1U
#define SELECT_FADEIN  2U

#endif
