#include <gb/gb.h>
#include "defines.h"

#include "gamestate.h"

UBYTE ticks;
UBYTE gamestate, level;
UBYTE joystate, oldjoystate;
UBYTE next_sprite, sprites_used;

void clearSprites() {
	UBYTE i;
	for(i = 0U; i < 40U; ++i) move_sprite(i, 0U, 0U);
	sprites_used = 0U;
	next_sprite = 0U;
}

void updateJoystate() {
	oldjoystate = joystate;
	joystate = joypad();
}

void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop) {
	move_sprite(next_sprite, x, y);
	set_sprite_tile(next_sprite, tile);
	set_sprite_prop(next_sprite, prop);

	sprites_used++;
	next_sprite++;
	if(next_sprite == 40U) next_sprite = 0U;
}

void clearRemainingSprites() {
	for(; sprites_used != 40U; ++sprites_used) {
		move_sprite(next_sprite++, 0, 0);
		if(next_sprite == 40U) next_sprite = 0U;
	}
	sprites_used = 0U;
}
