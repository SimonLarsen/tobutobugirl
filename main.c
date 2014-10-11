#include <gb/gb.h>

#include "game.h"
#include "title.h"
#include "input.h"
#include "gamestate.h"

#include "main.h"

void main() {
	clearCompleted();
	gamestate = GAMESTATE_GAME;
	while(1) {
		joystate = oldjoystate = 0U;

		switch(gamestate) {
			case GAMESTATE_TITLE:
				enterTitle();
				break;
			case GAMESTATE_GAME:
				enterGame();
				break;
		}
	}
}
