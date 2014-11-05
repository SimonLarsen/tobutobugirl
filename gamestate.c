#include <gb/gb.h>
#include "defines.h"

#include "gamestate.h"

void clearSprites() {
	UBYTE i;
	for(i = 0U; i < 40U; ++i) move_sprite(i, 0U, 0U);
}
