#include <gb/gb.h>
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "mmlgb/driver/notes.h"
#include "mmlgb/driver/freq.h"
#include "mmlgb/driver/noisefreq.h"
#include "mmlgb/driver/vib.h"

#include "data/sounds/sfx_blip.h"

UBYTE snd_active1, snd_active4;

UWORD snd_freq1; UBYTE snd_freq4;
UBYTE *snd_data1, *snd_data4;
UBYTE snd_octave1, snd_octave4;
UBYTE snd_length1, snd_length4;
UBYTE snd_volume1, snd_volume4;
UBYTE snd_env1, snd_env4;
UBYTE snd_duty1;
UBYTE snd_wait1, snd_wait4;
UWORD snd_target1, snd_target4;
UBYTE snd_slide1, snd_slide4;
UBYTE snd_vib_speed1;
UBYTE *snd_vib_table1;
UBYTE snd_vib_pos1;
UBYTE snd_noise_step;

void snd_init() {
	snd_active1 = snd_active4 = 0U;
}

void snd_play(UBYTE id) {
	UBYTE *data;

	switch(id) {
		case SFX_BLIP:
			data = sfx_blip_data;
			break;
	}
	snd_data1 = data + ((UWORD*)data)[CHN1_OFFSET];
	snd_data4 = data + ((UWORD*)data)[CHN4_OFFSET];

	if(*snd_data1 != T_EOF) {
		mus_disable1();
		snd_active1 = 1U;

		snd_wait1 = 0U;
		snd_octave1 = 4U;
		snd_length1 = 48U;
		snd_volume1 = 15U;
		snd_env1 = 3U;
		snd_slide1 = 0U;
		snd_vib_speed1 = 0U;
		NR51_REG |= 0x11U;
	}
	if(*snd_data4 != T_EOF) {
		mus_disable4();
		snd_active4 = 1U;

		snd_wait4 = 0U;
		snd_octave4 = 4U;
		snd_length4 = 48U;
		snd_volume4 = 15U;
		snd_env4 = 3U;
		snd_slide4 = 0U;
		snd_noise_step = 0U;
		NR51_REG |= 0x88U;
	}
}

void snd_update() {
	if(snd_active1) snd_update1();
	if(snd_active4) snd_update4();
}

void snd_update1() {
	UBYTE note;
	UWORD vib_freq;

	if(snd_slide1) {
		if(snd_target1 > snd_freq1) {
			snd_freq1 += snd_slide1;
			if(snd_freq1 > snd_target1) {
				snd_freq1 = snd_target1;
			}
		}
		else if(snd_target1 < snd_freq1) {
			snd_freq1 -= snd_slide1;
			if(snd_freq1 < snd_target1) {
				snd_freq1 = snd_target1;
			}
		}
		NR13_REG = (UBYTE)snd_freq1;
		NR14_REG = snd_freq1 >> 8;
	}

	if(snd_vib_speed1) {
		snd_vib_pos1 = (snd_vib_pos1 + snd_vib_speed1) & 63U;
		vib_freq = snd_freq1 - *snd_vib_table1 + snd_vib_table1[snd_vib_pos1];

		NR13_REG = (UBYTE)vib_freq;
		NR14_REG = vib_freq >> 8;
	}

	if(snd_wait1) {
		snd_wait1--;
		if(snd_wait1) return;
	}

	while(1U) {
		note = *snd_data1++;
		if(note & MUS_HAS_LENGTH || note <= T_WAIT) {
			if(note & MUS_HAS_LENGTH) {
				note ^= MUS_HAS_LENGTH;
				snd_wait1 = *snd_data1++;
			}
			else {
				snd_wait1 = snd_length1;
			}
			if(note == T_WAIT) {
				return;
			} else if(note == T_REST) {
				snd_freq1 = 0U;
				NR12_REG = 0U;
			} else {
				if(snd_slide1) {
					snd_target1 = freq[((snd_octave1-MUS_FIRST_OCTAVE) << 4) + note];
				} else {
					snd_freq1 = freq[((snd_octave1-MUS_FIRST_OCTAVE) << 4) + note];
				}
				NR12_REG = (snd_volume1 << 4) | snd_env1;
			}
			NR13_REG = (UBYTE)snd_freq1;
			NR14_REG = (snd_freq1 >> 8) | 0x80U;
			return;
		}
		switch(note) {
			case T_LENGTH:
				snd_length1 = *snd_data1++;
				break;
			case T_OCTAVE:
				snd_octave1 = *snd_data1++;
				break;
			case T_OCT_UP:
				snd_octave1++;
				break;
			case T_OCT_DOWN:
				snd_octave1--;
				break;
			case T_VOL:
				snd_volume1 = *snd_data1++;
				NR12_REG = (snd_volume1 << 4) | snd_env1;
				break;
			case T_ENV:
				snd_env1 = *snd_data1++;
				NR12_REG = (snd_volume1 << 4) | snd_env1;
				break;
			case T_WAVEDUTY:
				snd_duty1 = *snd_data1++;
				NR11_REG = snd_duty1 << 5;
				break;
			case T_PORTAMENTO:
				snd_slide1 = *snd_data1++;
				break;
			case T_VIBRATO:
				snd_vib_pos1 = 0U;
				note = *snd_data1++;
				snd_vib_speed1 = note & 15U;
				note = note >> 4;
				if(note == 1U) snd_vib_table1 = vib1;
				else if(note == 2U) snd_vib_table1 = vib2;
				else if(note == 3U) snd_vib_table1 = vib3;
				else snd_vib_table1 = vib4;
				break;
			case T_EOF:
				snd_active1 = 0U;
				mus_restore1();
				break;
		}
	}
}

void snd_update4() {
	UBYTE note;

	if(snd_slide4) {
		if(snd_target4 > snd_freq4) {
			snd_freq4 += snd_slide4;
			if(snd_freq4 > snd_target4) {
				snd_freq4 = snd_target4;
			}
		}
		else if(snd_target4 < snd_freq4) {
			snd_freq4 -= snd_slide4;
			if(snd_freq4 < snd_target4) {
				snd_freq4 = snd_target4;
			}
		}
		NR43_REG = snd_freq4 | snd_noise_step;
	}

	if(snd_wait4) {
		snd_wait4--;
		if(snd_wait4) return;
	}

	while(1U) {
		note = *snd_data4++;
		if(note & MUS_HAS_LENGTH || note <= T_WAIT) {
			if(note & MUS_HAS_LENGTH) {
				note ^= MUS_HAS_LENGTH;
				snd_wait4 = *snd_data4++;
			} else {
				snd_wait4 = snd_length4;
			}
			if(note == T_WAIT) {
				return;
			} else if(note == T_REST) {
				snd_freq4 = 0U;
				NR42_REG = 0U;
			} else {
				if(snd_slide4) {
					snd_target4 = noise_freq[((snd_octave4-MUS_NOISE_FIRST_OCTAVE) << 4) + note];
				} else {
					snd_freq4 = noise_freq[((snd_octave4-MUS_NOISE_FIRST_OCTAVE) << 4) + note];
				}
				NR42_REG = (snd_volume4 << 4) | snd_env4;
			}
			NR43_REG = snd_freq4 | snd_noise_step;
			NR44_REG = 0x80U;
			return;
		}
		switch(note) {
			case T_LENGTH:
				snd_length4 = *snd_data4++;
				break;
			case T_OCTAVE:
				snd_octave4 = *snd_data4++;
				break;
			case T_OCT_UP:
				snd_octave4++;
				break;
			case T_OCT_DOWN:
				snd_octave4--;
				break;
			case T_VOL:
				snd_volume4 = *snd_data4++;
				NR42_REG = (snd_volume4 << 4) | snd_env4;
				break;
			case T_ENV:
				snd_env4 = *snd_data4++;
				NR42_REG = (snd_volume4 << 4) | snd_env4;
				break;
			case T_PORTAMENTO:
				snd_slide4 = *snd_data4++;
				break;
			case T_NOISE_STEP:
				snd_noise_step = (*snd_data4++) << 3;
				break;
			case T_EOF:
				snd_active4 = 0U;
				mus_restore4();
				break;
		}
	}
}
