#ifndef GAMESTATE_H
#define GAMESTATE_H

void clearSprites();
void updateJoystate();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();

extern UBYTE ticks;
extern UBYTE gamestate, level;
extern UBYTE joystate, oldjoystate;
extern UBYTE next_sprite, sprites_used;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#define GAMESTATE_TITLE		0U
#define GAMESTATE_SELECT	1U
#define GAMESTATE_INGAME	2U
#define GAMESTATE_WINSCREEN	3U
#define GAMESTATE_HIGHSCORE	4U

#endif
