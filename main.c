#include <gb/gb.h>

#include "game.h"
#include "input.h"
#include "gamestate.h"

#include "main.h"

void main() {
	joystate = oldjoystate = 0U;
	while(1) {
		enterGame();
	}
}
