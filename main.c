#include <gb/gb.h>
#include "gamestate.h"
#include "main.h"
#include "ram.h"
#include "sound.h"
#include "mmlgb/driver/music.h"

#include "logos.h"
#include "intro.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "winscreen.h"
#include "highscore.h"
#include "unlocked.h"
#include "jukebox.h"
#include "ending.h"
#include "wipe.h"

const UBYTE RAM_SIG[8] = {'T','O','B','U','T','O','B','U'};

void initRAM(UBYTE force_clear) {
	UBYTE initialized, i;

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	// Check for signature
	initialized = 1U;
	for(i = 0U; i != 8U; ++i) {
		if(ram_data[RAM_SIG_ADDR + i] != RAM_SIG[i]) {
			initialized = 0U;
			break;
		}
	}

	// Initialize memory
	if(initialized == 0U || force_clear) {
		for(i = 0U; i != 64U; ++i) {
			ram_data[i] = 0U;
		}

		for(i = 0U; i != 8U; ++i) {
			ram_data[RAM_SIG_ADDR + i] = RAM_SIG[i];
		}

		ram_data[RAM_DASHCOUNTER] = 1U;
	}

	// Load values from ram
	for(levels_completed = 0U; levels_completed != 4U; ++levels_completed) {
		if(ram_data[levels_completed << 4] == 0U) break;
	}

	show_dashcounter = ram_data[RAM_DASHCOUNTER];

	DISABLE_RAM_MBC1;
}

void main() {
	initRAM(0U);
	snd_init();

	last_highscore_level = 0U;
	last_highscore_slot = 5U;
	last_progress = 0U;
	selection = level = 1U;

	joystate = oldjoystate = 0U;
	level = 1U;
	unlocked_bits = 0U;
	player_skin = 1U;
	ending_flags = 0U;
	
	gamestate = GAMESTATE_WINSCREEN;

	SWITCH_16_8_MODE_MBC1;
	add_TIM(updateMusic);
	set_interrupts(TIM_IFLAG | VBL_IFLAG);
	//set_interrupts(TIM_IFLAG);

	while(1U) {
		switch(gamestate) {
			case GAMESTATE_LOGOS:
				setGameBank(9U);
				enterLogos();
				break;
			case GAMESTATE_INGAME:
				setGameBank(1U);
				enterGame();
				break;
			case GAMESTATE_TITLE:
				setGameBank(8U);
				enterTitle();
				break;
			case GAMESTATE_SELECT:
				setGameBank(2U);
				enterSelect();
				break;
			case GAMESTATE_HIGHSCORE:
				setGameBank(2U);
				enterHighscore();
				break;
			case GAMESTATE_UNLOCKED:
				setGameBank(2U);
				enterUnlocked();
				break;
			case GAMESTATE_INTRO:
				setGameBank(3U);
				enterIntro();
				break;
			case GAMESTATE_ENDING:
				setGameBank(3U);
				enterEnding();
				break;
			case GAMESTATE_JUKEBOX:
				setGameBank(4U);
				enterJukebox();
				break;
			case GAMESTATE_WINSCREEN:
				setGameBank(7U);
				enterWinscreen();
				break;
			case GAMESTATE_WIPE:
				setGameBank(1U);
				enterWipe();
				break;
		}
	}
}
