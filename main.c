#include <gb/gb.h>

#include "gamestate.h"

#include "main.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "winscreen.h"
#include "highscore.h"
#include "ram.h"

const UBYTE RAM_SIG[8] = {'T','O','B','U','T','O','B','U'};

void initRAM() {
	UBYTE initialized, i;

	ENABLE_RAM_MBC1;
	SWITCH_4_32_MODE_MBC1;
	SWITCH_RAM_MBC1(0);

	initialized = 1U;
	for(i = 0U; i != 8U; ++i) {
		if(ram_data[SIG_ADDR + i] != RAM_SIG[i]) {
			initialized = 0U;
			break;
		}
	}

	if(initialized == 0U) {
		for(i = 0U; i != 42U; ++i) {
			ram_data[i] = 0U;
		}

		for(i = 0U; i != 8U; ++i) {
			ram_data[SIG_ADDR + i] = RAM_SIG[i];
		}
	}

	DISABLE_RAM_MBC1;
}

void main() {
	initRAM();

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
