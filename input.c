#include <gb/gb.h>
#include "input.h"

UBYTE joystate, oldjoystate;

void updateJoystate() {
	oldjoystate = joystate;
	joystate = joypad();
}
