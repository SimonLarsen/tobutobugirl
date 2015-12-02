#include <gb/gb.h>
#include "sound.h"
#include "notes.h"
#include "freq.h"
#include "noisefreq.h"
#include "music.h"

UBYTE snd_square_time;
UBYTE snd_noise_time;

void snd_init() {
	snd_square_time = 0U;
	snd_noise_time = 0U;
}

void snd_play(UBYTE id) {
	UWORD tmpfreq;
	disable_interrupts();
	switch(id) {
		case SFX_BLIP:
			mus_disable1();
			snd_square_time = 10U;
			tmpfreq = freq[48];
			NR11_REG = 0x40U;
			NR12_REG = 0xF1U;
			NR13_REG = (UBYTE)tmpfreq;
			NR14_REG = (tmpfreq >> 8) | 0x80U;

			delay(30U);
			NR11_REG = 0x0U;
			break;
	}
	enable_interrupts();
}

void snd_update() {
	if(snd_square_time) {
		snd_square_time--;
		if(!snd_square_time) {
			mus_restore1();
		}
	}

	if(snd_noise_time) {
		snd_noise_time--;
		if(!snd_noise_time) {
			mus_restore4();
		}
	}
}
