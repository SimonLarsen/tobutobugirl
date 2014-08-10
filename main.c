#include <gb/gb.h>

#include "game.h"
#include "title.h"
#include "gamestate.h"

#include "main.h"

UBYTE gamestate;

void main() {
	gamestate = GAMESTATE_TITLE;
	while(1) {
		switch(gamestate) {
			case GAMESTATE_TITLE:
				enterTitle();
				break;
			case GAMESTATE_LEVEL:
				break;
			case GAMESTATE_GAME:
				enterGame();
				break;
		}
	}
}
