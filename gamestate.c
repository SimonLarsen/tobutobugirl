#include <gb/gb.h>
#include "defines.h"

#include "gamestate.h"

UBYTE ticks;
UBYTE gamestate, level, selection;
UBYTE joystate, oldjoystate;
UBYTE next_sprite, sprites_used;
UBYTE elapsed_time, remaining_time, kills;

const UBYTE level_names[4][6] = {
	{29U, 13U, 25U, 28U, 15U, 29U},
	{26U, 22U, 11U, 19U, 24U, 29U},
	{13U, 22U, 25U, 31U, 14U, 29U},
	{29U, 26U, 11U, 13U, 15U, 10U}
};

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
