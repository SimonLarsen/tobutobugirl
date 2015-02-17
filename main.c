#include <gb/gb.h>

#include "game.h"
#include "input.h"
#include "gamestate.h"

#include "main.h"
#include "title.h"

void main() {
	joystate = oldjoystate = 0U;

	enterTitle();
	while(1) {
		enterGame();
	}
}
