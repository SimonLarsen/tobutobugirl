#ifndef GAMESTATE_H
#define GAMESTATE_H

void clearSprites();
void updateJoystate();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();

extern UBYTE ticks;
extern UBYTE gamestate, level, selection;
extern UBYTE joystate, oldjoystate;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#define GAMESTATE_TITLE		0U
#define GAMESTATE_SELECT	1U
#define GAMESTATE_INGAME	2U
#define GAMESTATE_WINSCREEN	3U
#define GAMESTATE_HIGHSCORE	4U

extern const UBYTE level_names[4][6];

#endif
