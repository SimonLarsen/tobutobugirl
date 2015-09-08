#include <gb/gb.h>
#include "gamestate.h"
#include "main.h"

#include "intro.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "winscreen.h"
#include "highscore.h"
#include "ram.h"
#include "music.h"
#include "sound.h"

const UBYTE RAM_SIG[8] = {'T','O','B','U','T','O','B','U'};

void initRAM() {
	UBYTE initialized, i;

	ENABLE_RAM_MBC1;
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

	for(levels_completed = 0U; levels_completed != 3U; ++levels_completed) {
		if(ram_data[levels_completed << 4] == 0U) break;
	}

	DISABLE_RAM_MBC1;
}

void updateMusic() {
	SWITCH_ROM_MBC1(music_bank);
	mus_update();
	SWITCH_ROM_MBC1(game_bank);
}

void main() {
	initRAM();
	snd_init();

	last_highscore_level = 0U;
	last_highscore_slot = 5U;

	joystate = oldjoystate = 0U;
	level = 1U;
	
	gamestate = GAMESTATE_INTRO;

	SWITCH_16_8_MODE_MBC1;
	add_TIM(updateMusic);
	set_interrupts(TIM_IFLAG | VBL_IFLAG);

	while(1U) {
		switch(gamestate) {
			case GAMESTATE_INTRO:
				setGameBank(6);
				enterIntro();
				break;
			case GAMESTATE_TITLE:
				setGameBank(3);
				enterTitle();
				break;
			case GAMESTATE_SELECT:
				setGameBank(2);
				enterSelect();
				break;
			case GAMESTATE_INGAME:
				setGameBank(1);
				enterGame();
				break;
			case GAMESTATE_WINSCREEN:
				setGameBank(3);
				enterWinscreen();
				break;
			case GAMESTATE_HIGHSCORE:
				setGameBank(2);
				enterHighscore();
				break;
		}
	}
}
