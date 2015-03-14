#include <gb/gb.h>

#include "gamestate.h"

#include "main.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "winscreen.h"
#include "highscore.h"

void initRAM() {
	ENABLE_RAM_MBC1;
	SWITCH_4_32_MODE_MBC1;

	SWITCH_RAM_MBC1(0);

	DISABLE_RAM_MBC1;
}

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
			case GAMESTATE_WINSCREEN:
				enterWinscreen();
				break;
			case GAMESTATE_HIGHSCORE:
				enterHighscore();
				break;
		}
	}
}
