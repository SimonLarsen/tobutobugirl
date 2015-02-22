#ifndef GAMESTATE_H
#define GAMESTATE_H

void clearSprites();
void updateJoystate();

extern UBYTE ticks;
extern UBYTE joystate, oldjoystate;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#endif
