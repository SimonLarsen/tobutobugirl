#include <gb/gb.h>

#include "gamestate.h"

#include "main.h"
#include "title.h"
#include "select.h"
#include "game.h"

void main() {
	joystate = oldjoystate = 0U;

	enterTitle();
	enterSelect();
	while(1) {
		enterGame();
	}
}
