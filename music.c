#include <gb/gb.h>
#include <string.h>
#include "music.h"
#include "notes.h"
#include "freq.h"
#include "noisefreq.h"

#define MAX_REPEATS 4

UBYTE mus_paused;
UBYTE *mus_song;

UBYTE mus_enabled1, mus_enabled4;
UWORD mus_freq1, mus_freq2, mus_freq3;
UBYTE mus_freq4;
UBYTE *mus_data1, *mus_data2, *mus_data3, *mus_data4;
UBYTE *mus_loop1, *mus_loop2, *mus_loop3, *mus_loop4;
UBYTE mus_octave1, mus_octave2, mus_octave3, mus_octave4;
UBYTE mus_length1, mus_length2, mus_length3, mus_length4;
UBYTE mus_volume1, mus_volume2, mus_volume3, mus_volume4;
UBYTE mus_env1, mus_env2, mus_env4;
UBYTE mus_duty1, mus_duty2;
UBYTE mus_wait1, mus_wait2, mus_wait3, mus_wait4;
UWORD mus_target1, mus_target2, mus_target4;
UBYTE mus_slide1, mus_slide2, mus_slide4;
UBYTE mus_vib_speed1, mus_vib_speed2;
UBYTE *mus_vib_table1, *mus_vib_table2;
UBYTE mus_vib_pos1, mus_vib_pos2;
UBYTE mus_vib_delay1, mus_vib_delay2;
UBYTE mus_noise_step;
UBYTE mus_po1, mus_po2, mus_po3;

UBYTE *mus_rep1[MAX_REPEATS], *mus_rep2[MAX_REPEATS], *mus_rep3[MAX_REPEATS], *mus_rep4[MAX_REPEATS];
UBYTE mus_repeats1[MAX_REPEATS], mus_repeats2[MAX_REPEATS], mus_repeats3[MAX_REPEATS], mus_repeats4[MAX_REPEATS];
UBYTE mus_rep_depth1, mus_rep_depth2, mus_rep_depth3, mus_rep_depth4;

const UBYTE vib1[64] = {
	1U, 1U, 1U, 1U, 1U, 1U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U,
	2U, 2U, 2U, 2U, 2U, 2U, 2U, 2U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U,
	0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
	0U, 0U, 1U, 1U, 1U, 1U, 1U
};

const UBYTE vib2[64] = {
	2U, 2U, 2U, 3U, 3U, 3U, 3U, 3U, 3U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U,
	4U, 4U, 4U, 4U, 4U, 3U, 3U, 3U, 3U, 3U, 3U, 2U, 2U, 2U, 2U, 2U, 1U, 1U, 1U,
	1U, 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U,
	1U, 1U, 1U, 1U, 1U, 2U, 2U
};

const UBYTE vib3[64] = {
	4U, 4U, 5U, 5U, 6U, 6U, 6U, 7U, 7U, 7U, 7U, 8U, 8U, 8U, 8U, 8U, 8U, 8U, 8U,
	8U, 8U, 8U, 7U, 7U, 7U, 7U, 6U, 6U, 6U, 5U, 5U, 4U, 4U, 4U, 3U, 3U, 2U, 2U,
	2U, 1U, 1U, 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 1U, 1U,
	1U, 2U, 2U, 2U, 3U, 3U, 4U
};

const UBYTE vib4[64] = {
	8U, 9U, 10U, 10U, 11U, 12U, 12U, 13U, 14U, 14U, 15U, 15U, 15U, 16U, 16U,
	16U, 16U, 16U, 16U, 16U, 15U, 15U, 15U, 14U, 14U, 13U, 12U, 12U, 11U, 10U,
	10U, 9U, 8U, 7U, 6U, 6U, 5U, 4U, 4U, 3U, 2U, 2U, 1U, 1U, 1U, 0U, 0U, 0U,
	0U, 0U, 0U, 0U, 1U, 1U, 1U, 2U, 2U, 3U, 4U, 4U, 5U, 6U, 6U, 7U
};

const UBYTE wave_volume[4] = {
	0U, 96U, 64U, 32U
};

void mus_init(UBYTE *song_data) {
	UBYTE i;

	NR52_REG = 0x80U; // Enable sound
	NR51_REG = 0xFFU;
	NR50_REG = 0xFFU;

	// Kill sound
	NR10_REG = 0U;
	NR12_REG = 0U;
	NR22_REG = 0U;
	NR30_REG = 0U;
	NR32_REG = 0U;
	NR42_REG = 0U;

	// Setup timer
	TAC_REG = 0x04U; // TAC clock = 4096 Hz
	TMA_REG = 255U - 51U; // Default to ~150 bpm

	// Setup data
	mus_paused = 0U;

	mus_song = song_data;
	mus_data1 = mus_loop1 = mus_song + ((UWORD*)mus_song)[CHN1_OFFSET];
	mus_data2 = mus_loop2 = mus_song + ((UWORD*)mus_song)[CHN2_OFFSET];
	mus_data3 = mus_loop3 = mus_song + ((UWORD*)mus_song)[CHN3_OFFSET];
	mus_data4 = mus_loop4 = mus_song + ((UWORD*)mus_song)[CHN4_OFFSET];

	mus_enabled1 = mus_enabled4 = 0x80U;
	mus_wait1 = mus_wait2 = mus_wait3 = mus_wait4 = 0U;
	mus_octave1 = mus_octave2 = mus_octave3 = mus_octave4 = 4U;
	mus_length1 = mus_length2 = mus_length3 = mus_length4 = 48U;
	mus_volume1 = mus_volume2 = mus_volume4 = 15U;
	mus_volume3 = 1U;
	mus_env1 = mus_env2 = mus_env4 = 3U;
	mus_rep_depth1 = mus_rep_depth2 = mus_rep_depth3 = mus_rep_depth4 = 255U;
	mus_slide1 = mus_slide2 = mus_slide4 = 0U;
	mus_vib_speed1 = mus_vib_speed2 = 0U;
	mus_noise_step = 0U;
	mus_po1 = mus_po2 = mus_po3 = 128U;

	for(i = 0U; i != MAX_REPEATS; ++i) {
		mus_repeats1[i] = 0U;
		mus_repeats2[i] = 0U;
		mus_repeats3[i] = 0U;
		mus_repeats4[i] = 0U;
	}
}

void mus_setPaused(UBYTE p) {
	mus_paused = p;

	if(mus_paused) {
		NR12_REG = NR22_REG = NR32_REG = NR42_REG = 0U;
		NR14_REG = NR24_REG = NR34_REG = NR44_REG = 0x80U;
	}
}

void mus_togglePaused() {
	mus_setPaused(mus_paused ^ 1U);
}

void mus_disable1() {
	mus_enabled1 = 0U;
}

void mus_disable4() {
	mus_enabled4 = 0U;
}

void mus_restore1() {
	mus_enabled1 = 0x80U;
	NR10_REG = 0U;
	NR11_REG = mus_duty1 << 5;
	NR12_REG = (mus_volume1 << 4) | mus_env1;
}

void mus_restore4() {
	mus_enabled4 = 0x80U;
	NR42_REG = (mus_volume4 << 4) | mus_env4;
}

void mus_update() {
	if(mus_paused) return;

	mus_update1();
	mus_update2();
	mus_update3();
	mus_update4();
}

void mus_update1() {
	UBYTE note;
	UWORD vib_freq;

	if(mus_slide1) {
		if(mus_target1 > mus_freq1) {
			mus_freq1 += mus_slide1;
			if(mus_freq1 > mus_target1) {
				mus_freq1 = mus_target1;
			}
		}
		else if(mus_target1 < mus_freq1) {
			mus_freq1 -= mus_slide1;
			if(mus_freq1 < mus_target1) {
				mus_freq1 = mus_target1;
			}
		}
		NR13_REG = (UBYTE)mus_freq1;
		NR14_REG = mus_freq1 >> 8;
	}

	if(mus_vib_delay1) {
		mus_vib_delay1--;
	}
	else if(mus_vib_speed1) {
		mus_vib_pos1 = (mus_vib_pos1 + mus_vib_speed1) & 63U;
		vib_freq = mus_freq1 - *mus_vib_table1 + mus_vib_table1[mus_vib_pos1];

		NR13_REG = (UBYTE)vib_freq;
		NR14_REG = vib_freq >> 8;
	}

	if(mus_wait1) {
		mus_wait1--;
		if(mus_wait1) return;
	}

	while(1U) {
		note = *mus_data1++;
		if(note & MUS_HAS_LENGTH || note <= T_WAIT) {
			if(note & MUS_HAS_LENGTH) {
				note ^= MUS_HAS_LENGTH;
				mus_wait1 = *mus_data1++;
			}
			else {
				mus_wait1 = mus_length1;
			}
			if(note == T_WAIT) {
				return;
			} else if(note == T_REST) {
				mus_freq1 = 0U;
				NR12_REG = 0U;
			} else {
				if(mus_slide1) {
					mus_target1 = freq[((mus_octave1-MUS_FIRST_OCTAVE) << 4) + note] + mus_po1 - 128U;
				} else {
					mus_freq1 = freq[((mus_octave1-MUS_FIRST_OCTAVE) << 4) + note] + mus_po1 - 128U;
				}
				NR12_REG = (mus_volume1 << 4) | mus_env1;
			}
			NR13_REG = (UBYTE)mus_freq1;
			NR14_REG = mus_enabled1 | (mus_freq1 >> 8);
			return;
		}
		switch(note) {
			case T_LENGTH:
				mus_length1 = *mus_data1++;
				break;
			case T_OCTAVE:
				mus_octave1 = *mus_data1++;
				break;
			case T_OCT_UP:
				mus_octave1++;
				break;
			case T_OCT_DOWN:
				mus_octave1--;
				break;
			case T_VOL:
				mus_volume1 = *mus_data1++;
				NR12_REG = (mus_volume1 << 4) | mus_env1;
				break;
			case T_ENV:
				mus_env1 = *mus_data1++;
				NR12_REG = (mus_volume1 << 4) | mus_env1;
				break;
			case T_WAVEDUTY:
				mus_duty1 = *mus_data1++;
				NR11_REG = mus_duty1 << 5;
				break;
			case T_PAN:
				note = *mus_data1++;
				NR51_REG = (NR51_REG & 0xEEU) | note; // 11101110
				break;
			case T_PORTAMENTO:
				mus_slide1 = *mus_data1++;
				break;
			case T_VIBRATO:
				mus_vib_pos1 = 0U;
				note = *mus_data1++;
				mus_vib_speed1 = note & 15U;
				note = note >> 4;
				if(note == 1U) mus_vib_table1 = vib1;
				else if(note == 2U) mus_vib_table1 = vib2;
				else if(note == 3U) mus_vib_table1 = vib3;
				else mus_vib_table1 = vib4;
				mus_vib_delay1 = 0U;
				break;
			case T_VIBRATO_DELAY:
				mus_vib_pos1 = 0U;
				note = *mus_data1++;
				mus_vib_speed1 = note & 15U;
				note = note >> 4;
				if(note == 1U) mus_vib_table1 = vib1;
				else if(note == 2U) mus_vib_table1 = vib2;
				else if(note == 3U) mus_vib_table1 = vib3;
				else mus_vib_table1 = vib4;
				mus_vib_delay1 = *mus_data1++;
				break;
			case T_REP_START:
				mus_rep_depth1++;
				mus_rep1[mus_rep_depth1] = mus_data1;
				break;
			case T_REP_END:
				note = *mus_data1++;
				if(!mus_repeats1[mus_rep_depth1]) {
					mus_repeats1[mus_rep_depth1] = note;
					mus_data1 = mus_rep1[mus_rep_depth1];
				}
				mus_repeats1[mus_rep_depth1]--;
				if(mus_repeats1[mus_rep_depth1]) {
					mus_data1 = mus_rep1[mus_rep_depth1];
				} else {
					mus_rep_depth1--;
				}
				break;
			case T_LOOP:
				mus_loop1 = mus_data1;
				break;
			case T_TEMPO:
				TMA_REG = *mus_data1++;
				break;
			case T_PITCH_OFFSET:
				mus_po1 = *mus_data1++;
				break;
			case T_EOF:
				mus_data1 = mus_loop1;
				if(*mus_data1 == T_EOF) {
					mus_wait1 = 255U;
					return;
				}
				break;
		}
	}
}

void mus_update2() {
	UBYTE note;
	UWORD vib_freq;

	if(mus_slide2) {
		if(mus_target2 > mus_freq2) {
			mus_freq2 += mus_slide2;
			if(mus_freq2 > mus_target2) {
				mus_freq2 = mus_target2;
			}
		}
		else if(mus_target2 < mus_freq2) {
			mus_freq2 -= mus_slide2;
			if(mus_freq2 < mus_target2) {
				mus_freq2 = mus_target2;
			}
		}
		NR23_REG = (UBYTE)mus_freq2;
		NR24_REG = mus_freq2 >> 8;
	}

	if(mus_vib_delay2) {
		mus_vib_delay2--;
	}
	else if(mus_vib_speed2) {
		mus_vib_pos2 = (mus_vib_pos2 + mus_vib_speed2) & 63U;
		vib_freq = mus_freq2 - *mus_vib_table2 + mus_vib_table2[mus_vib_pos2];

		NR23_REG = (UBYTE)vib_freq;
		NR24_REG = vib_freq >> 8;
	}

	if(mus_wait2) {
		mus_wait2--;
		if(mus_wait2) return;
	}

	while(1U) {
		note = *mus_data2++;
		if(note & MUS_HAS_LENGTH || note <= T_WAIT) {
			if(note & MUS_HAS_LENGTH) {
				note ^= MUS_HAS_LENGTH;
				mus_wait2 = *mus_data2++;
			}
			else {
				mus_wait2 = mus_length2;
			}
			if(note == T_WAIT) {
				return;
			} else if(note == T_REST) {
				mus_freq2 = 0U;
				NR22_REG = 0U;
			} else {
				if(mus_slide2) {
					mus_target2 = freq[((mus_octave2-MUS_FIRST_OCTAVE) << 4) + note] + mus_po2 - 128U;
				} else {
					mus_freq2 = freq[((mus_octave2-MUS_FIRST_OCTAVE) << 4) + note] + mus_po2 - 128U;
				}
				NR22_REG = (mus_volume2 << 4) | mus_env2;
			}
			NR23_REG = (UBYTE)mus_freq2;
			NR24_REG = 0x80U | (mus_freq2 >> 8);
			return;
		}
		switch(note) {
			case T_LENGTH:
				mus_length2 = *mus_data2++;
				break;
			case T_OCTAVE:
				mus_octave2 = *mus_data2++;
				break;
			case T_OCT_UP:
				mus_octave2++;
				break;
			case T_OCT_DOWN:
				mus_octave2--;
				break;
			case T_VOL:
				mus_volume2 = *mus_data2++;
				NR22_REG = (mus_volume2 << 4) | mus_env2;
				break;
			case T_ENV:
				mus_env2 = *mus_data2++;
				NR22_REG = (mus_volume2 << 4) | mus_env2;
				break;
			case T_WAVEDUTY:
				mus_duty2 = *mus_data2++;
				NR21_REG = mus_duty2 << 5;
				break;
			case T_PAN:
				note = *mus_data2++;
				NR51_REG = (NR51_REG & 0xDDU) | (note << 1); // 11011101
				break;
			case T_PORTAMENTO:
				mus_slide2 = *mus_data2++;
				break;
			case T_VIBRATO:
				mus_vib_pos2 = 0U;
				note = *mus_data2++;
				mus_vib_speed2 = note & 15U; // 0b1111
				note = note >> 4;
				if(note == 1U) mus_vib_table2 = vib1;
				else if(note == 2U) mus_vib_table2 = vib2;
				else if(note == 3U) mus_vib_table2 = vib3;
				else mus_vib_table2 = vib4;
				mus_vib_delay2 = 0U;
				break;
			case T_VIBRATO_DELAY:
				mus_vib_pos2 = 0U;
				note = *mus_data2++;
				mus_vib_speed2 = note & 15U;
				note = note >> 4;
				if(note == 1U) mus_vib_table2 = vib1;
				else if(note == 2U) mus_vib_table2 = vib2;
				else if(note == 3U) mus_vib_table2 = vib3;
				else mus_vib_table2 = vib4;
				mus_vib_delay2 = *mus_data2++;
				break;
			case T_REP_START:
				mus_rep_depth2++;
				mus_rep2[mus_rep_depth2] = mus_data2;
				break;
			case T_REP_END:
				note = *mus_data2++;
				if(!mus_repeats2[mus_rep_depth2]) {
					mus_repeats2[mus_rep_depth2] = note;
					mus_data2 = mus_rep2[mus_rep_depth2];
				}
				mus_repeats2[mus_rep_depth2]--;
				if(mus_repeats2[mus_rep_depth2]) {
					mus_data2 = mus_rep2[mus_rep_depth2];
				} else {
					mus_rep_depth2--;
				}
				break;
			case T_LOOP:
				mus_loop2 = mus_data2;
				break;
			case T_TEMPO:
				TMA_REG = *mus_data2++;
				break;
			case T_PITCH_OFFSET:
				mus_po2 = *mus_data2++;
				break;
			case T_EOF:
				mus_data2 = mus_loop2;
				if(*mus_data2 == T_EOF) {
					mus_wait2 = 255U;
					return;
				}
				break;
		}
	}
}

void mus_update3() {
	UBYTE note;

	if(mus_wait3) {
		mus_wait3--;
		if(mus_wait3) return;
	}

	while(1U) {
		note = *mus_data3++;
		if(note & MUS_HAS_LENGTH || note <= T_WAIT) {
			if(note & MUS_HAS_LENGTH) {
				note ^= MUS_HAS_LENGTH;
				mus_wait3 = *mus_data3++;
			} else {
				mus_wait3 = mus_length3;
			}
			if(note == T_WAIT) {
				return;
			} else if(note == T_REST) {
				mus_freq3 = 0U;
				NR32_REG = 0U;
			} else {
				mus_freq3 = freq[((mus_octave3-MUS_FIRST_OCTAVE) << 4) + note] + mus_po3 - 128U;
				NR32_REG = mus_volume3;
			}
			NR30_REG = 0x0U;
			NR30_REG = 0x80U;
			NR33_REG = (UBYTE)mus_freq3;
			NR34_REG = 0x80U | (mus_freq3 >> 8);
			return;
		}
		switch(note) {
			case T_LENGTH:
				mus_length3 = *mus_data3++;
				break;
			case T_OCTAVE:
				mus_octave3 = *mus_data3++;
				break;
			case T_OCT_UP:
				mus_octave3++;
				break;
			case T_OCT_DOWN:
				mus_octave3--;
				break;
			case T_VOL:
				note = *mus_data3++;
				mus_volume3 = wave_volume[note];
				break;
			case T_WAVE:
				note = *mus_data3++;
				NR30_REG = 0x0U;
				memcpy(0xFF30, mus_song + WAVE_OFFSET + (note << 4), 16U);
				NR30_REG = 0x80U;
				break;
			case T_PAN:
				note = *mus_data3++;
				NR51_REG = (NR51_REG & 0xBBU) | (note << 2); // 10111011
				break;
			case T_REP_START:
				mus_rep_depth3++;
				mus_rep3[mus_rep_depth3] = mus_data3;
				break;
			case T_REP_END:
				note = *mus_data3++;
				if(!mus_repeats3[mus_rep_depth3]) {
					mus_repeats3[mus_rep_depth3] = note;
					mus_data3 = mus_rep3[mus_rep_depth3];
				}
				mus_repeats3[mus_rep_depth3]--;
				if(mus_repeats3[mus_rep_depth3]) {
					mus_data3 = mus_rep3[mus_rep_depth3];
				} else {
					mus_rep_depth3--;
				}
				break;
			case T_LOOP:
				mus_loop3 = mus_data3;
				break;
			case T_TEMPO:
				TMA_REG = *mus_data3++;
				break;
			case T_PITCH_OFFSET:
				mus_po3 = *mus_data3++;
				break;
			case T_EOF:
				mus_data3 = mus_loop3;
				if(*mus_data3 == T_EOF) {
					mus_wait3 = 255U;
					return;
				}
				break;
		}
	}
}

void mus_update4() {
	UBYTE note;

	if(mus_slide4) {
		if(mus_target4 > mus_freq4) {
			mus_freq4 += mus_slide4;
			if(mus_freq4 > mus_target4) {
				mus_freq4 = mus_target4;
			}
		}
		else if(mus_target4 < mus_freq4) {
			mus_freq4 -= mus_slide4;
			if(mus_freq4 < mus_target4) {
				mus_freq4 = mus_target4;
			}
		}
		NR43_REG = mus_freq4 | mus_noise_step;
	}

	if(mus_wait4) {
		mus_wait4--;
		if(mus_wait4) return;
	}

	while(1U) {
		note = *mus_data4++;
		if(note & MUS_HAS_LENGTH || note <= T_WAIT) {
			if(note & MUS_HAS_LENGTH) {
				note ^= MUS_HAS_LENGTH;
				mus_wait4 = *mus_data4++;
			} else {
				mus_wait4 = mus_length4;
			}
			if(note == T_WAIT) {
				return;
			} else if(note == T_REST) {
				mus_freq4 = 0U;
				NR42_REG = 0U;
			} else {
				if(mus_slide4) {
					mus_target4 = noise_freq[((mus_octave4-MUS_NOISE_FIRST_OCTAVE) << 4) + note];
				} else {
					mus_freq4 = noise_freq[((mus_octave4-MUS_NOISE_FIRST_OCTAVE) << 4) + note];
				}
				NR42_REG = (mus_volume4 << 4) | mus_env4;
			}
			NR43_REG = mus_freq4 | mus_noise_step;
			NR44_REG = mus_enabled4;
			return;
		}
		switch(note) {
			case T_LENGTH:
				mus_length4 = *mus_data4++;
				break;
			case T_OCTAVE:
				mus_octave4 = *mus_data4++;
				break;
			case T_OCT_UP:
				mus_octave4++;
				break;
			case T_OCT_DOWN:
				mus_octave4--;
				break;
			case T_VOL:
				mus_volume4 = *mus_data4++;
				NR42_REG = (mus_volume4 << 4) | mus_env4;
				break;
			case T_ENV:
				mus_env4 = *mus_data4++;
				NR42_REG = (mus_volume4 << 4) | mus_env4;
				break;
			case T_PAN:
				note = *mus_data4++;
				NR51_REG = (NR51_REG & 0x77U) | (note << 3); // 01110111
				break;
			case T_PORTAMENTO:
				mus_slide4 = *mus_data4++;
				break;
			case T_REP_START:
				mus_rep_depth4++;
				mus_rep4[mus_rep_depth4] = mus_data4;
				break;
			case T_REP_END:
				note = *mus_data4++;
				if(!mus_repeats4[mus_rep_depth4]) {
					mus_repeats4[mus_rep_depth4] = note;
					mus_data4 = mus_rep4[mus_rep_depth4];
				}
				mus_repeats4[mus_rep_depth4]--;
				if(mus_repeats4[mus_rep_depth4]) {
					mus_data4 = mus_rep4[mus_rep_depth4];
				} else {
					mus_rep_depth4--;
				}
				break;
			case T_LOOP:
				mus_loop4 = mus_data4;
				break;
			case T_TEMPO:
				TMA_REG = *mus_data4++;
				break;
			case T_NOISE_STEP:
				mus_noise_step = (*mus_data4++) << 3;
				break;
			case T_EOF:
				mus_data4 = mus_loop4;
				if(*mus_data4 == T_EOF) {
					mus_wait4 = 255U;
					return;
				}
				break;
		}
	}
}
