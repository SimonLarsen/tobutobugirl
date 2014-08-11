#include <gb/gb.h>
#include "defines.h"

#include "gamestate.h"

UBYTE gamestate;
UBYTE level;
UBYTE completed[NUM_LEVELS];

void clearCompleted() {
	UBYTE i;
	for(i = 0U; i < NUM_LEVELS; ++i) {
		completed[i] = 0U;
	}
}
void clearSprites() {
	UBYTE i;
	for(i = 0U; i < 40U; ++i) move_sprite(i, 0U, 0U);
}
