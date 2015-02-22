#include <gb/gb.h>
#include "defines.h"

#include "gamestate.h"

UBYTE ticks;
UBYTE level;
UBYTE joystate, oldjoystate;

void clearSprites() {
	UBYTE i;
	for(i = 0U; i < 40U; ++i) move_sprite(i, 0U, 0U);
}

void updateJoystate() {
	oldjoystate = joystate;
	joystate = joypad();
}
