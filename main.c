#include <gb/gb.h>

#include "gamestate.h"

#include "main.h"
#include "title.h"
#include "select.h"
#include "game.h"

void main() {
	joystate = oldjoystate = 0U;
	level = 1U;

	gamestate = GAMESTATE_TITLE;

	while(1U) {
		switch(gamestate) {
			case GAMESTATE_TITLE:
				enterTitle();
				break;
			case GAMESTATE_SELECT:
				enterSelect();
				break;
			case GAMESTATE_INGAME:
				enterGame();
				break;
		}
	}
}
