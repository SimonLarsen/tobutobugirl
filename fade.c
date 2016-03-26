#include <gb/gb.h>
#include "sound.h"
#include "fade.h"

const UINT8 fadePals[] = {
	0xE4U, // 11100100
	0x90U, // 10010000
	0x40U, // 01000000
	0x00U  // 00000000
};

const UINT8 spriteFadePals[] = {
	0xD0U, // 11010000
	0x80U, // 10000000
	0x40U, // 01000000
	0x00U  // 00000000
};

void fadeToWhite(UBYTE delay) {
	UINT8 i, j;
	for(i = 1U; i != 4U; ++i) {
		BGP_REG = spriteFadePals[i];
		for(j = 0U; j != delay; ++j) {
			snd_update();
			wait_vbl_done();
		}
	}
}

void fadeSpritesToWhite(UBYTE delay) {
	UINT8 i, j;
	for(i = 1U; i != 4U; ++i) {
		OBP0_REG = fadePals[i];
		for(j = 0U; j != delay; ++j) {
			snd_update();
			wait_vbl_done();
		}
	}
}

void fadeFromWhite(UBYTE delay) {
	UINT8 i, j;

	for(i = 3U; i != 0U; --i) {
		BGP_REG = fadePals[i];
		for(j = 0U; j != delay; ++j) {
			snd_update();
			wait_vbl_done();
		}
	}

	BGP_REG = fadePals[0U];
}
