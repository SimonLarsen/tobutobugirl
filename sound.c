#include <gb/gb.h>
#include "sound.h"
#include "notes.h"
#include "freq.h"
#include "noisefreq.h"
#include "music.h"

/*
 * sfx_data
 * Contains 8 bytes per sound effect:
 *  Byte 0: Sound type (SND_TYPE_SQUARE, SND_TYPE_NOISE)
 *  Byte 1: Length in updates (1/60 second)
 *  Byte 2: Volume (0-15)
 *  Byte 3: Envelope (0-3)
 *  Byte 4: Octave
 *  Byte 5: Note (see notes.h)
 *  Byte 6: Duty (SQUARE only)
 *  Byte 7: Sweep register (SQUARE only)
 */
const UBYTE sfx_data[] = {
	// SND_CONFIRM
	SND_TYPE_SQUARE,
	4U,
	15U,
	0xF1U, // 11110001
	5U,
	T_D,
	1U,
	0x77U
};

UBYTE snd_square_time;
UBYTE snd_noise_time;

void snd_init() {
	snd_square_time = 0U;
	snd_noise_time = 0U;
}

void snd_play(UBYTE id) {
	UWORD frequency;
	UBYTE *data;
	data = &sfx_data[id << 3];

	if(data[0] == SND_TYPE_SQUARE) {
		mus_disable1();
		snd_square_time = data[1];
		frequency = freq[((data[4]-MUS_FIRST_OCTAVE) << 4) + data[5]];
		NR10_REG = data[7];
		NR11_REG = data[6] << 5;
		NR12_REG = (data[2] << 4) | data[3];
		NR13_REG = (UBYTE)frequency;
		NR14_REG = 0x80U | (frequency >> 8);
	} 
	else if(data[0] == SND_TYPE_NOISE) {
		mus_disable4();
		snd_noise_time = data[1];
		NR42_REG = (data[2] << 4) | data[3];
		NR43_REG = noise_freq[((data[4]-MUS_NOISE_FIRST_OCTAVE) << 4) + data[5]];
		NR44_REG = 0x80U;
	}
}

void snd_update() {
	if(snd_square_time) {
		snd_square_time--;
		if(!snd_square_time) {
			NR14_REG = 0U;
			mus_restore1();
		}
	}

	if(snd_noise_time) {
		snd_noise_time--;
		if(!snd_noise_time) {
			NR44_REG = 0U;
			mus_restore4();
		}
	}
}
