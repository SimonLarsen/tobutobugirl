#include <gb/gb.h>
#include "sound.h"
#include "gamestate.h"
#include "mmlgb/driver/music.h"
#include "mmlgb/driver/notes.h"
#include "mmlgb/driver/freq.h"
#include "mmlgb/driver/noisefreq.h"
#include "mmlgb/driver/vib.h"

extern UBYTE sfx_bump_data;
extern UBYTE sfx_bump_alien_data;
extern UBYTE sfx_cat_disable_data;
extern UBYTE sfx_cat_enable_data;
extern UBYTE sfx_dash_data;
extern UBYTE sfx_highscore_switch_data;
extern UBYTE sfx_jetpack_data;
extern UBYTE sfx_menu_cancel_data;
extern UBYTE sfx_menu_confirm_data;
extern UBYTE sfx_menu_locked_data;
extern UBYTE sfx_menu_switch_data;
extern UBYTE sfx_player_die_data;
extern UBYTE sfx_stomp_alien_data;
extern UBYTE sfx_stomp_bat_data;
extern UBYTE sfx_stomp_bird_data;
extern UBYTE sfx_stomp_ghost_data;
extern UBYTE sfx_time_low_data;
extern UBYTE sfx_time_out_data;
extern UBYTE sfx_time_pickup_data;
extern UBYTE sfx_warp_start_data;

UBYTE snd_active1, snd_active4;
UBYTE snd_priority1, snd_priority4;

UWORD snd_freq1; UBYTE snd_freq4;
UBYTE *snd_data1, *snd_data4;
UBYTE snd_octave1, snd_octave4;
UBYTE snd_length1, snd_length4;
UBYTE snd_volume1, snd_volume4;
UBYTE snd_env1, snd_env4;
UBYTE snd_wait1, snd_wait4;
UWORD snd_target1, snd_target4;
UBYTE snd_slide1, snd_slide4;
UBYTE snd_vib_speed1;
UBYTE *snd_vib_table1;
UBYTE snd_vib_pos1;
UBYTE snd_noise_step;
UBYTE snd_po1;

const UBYTE sfx_priority[SFX_COUNT] = {
	8U, // SFX_BUMP
	8U, // SFX_BUMP_ALIEN
	8U, // SFX_CAT_DISABLE
	8U, // SFX_CAT_ENABLE
	7U, // SFX_DASH
	8U, // SFX_HIGHSCORE_SWITCH
	7U, // SFX_JETPACK
	8U, // SFX_MENU_CANCEL
	8U, // SFX_MENU_CONFIRM
	8U, // SFX_MENU_LOCKED
	8U, // SFX_MENU_SWITCH
	9U, // SFX_PLAYER_DIE
	8U, // SFX_STOMP_ALIEN
	8U, // SFX_STOMP_BAT
	8U, // SFX_STOMP_BIRD
	8U, // SFX_STOMP_GHOST
	9U, // SFX_TIME_LOW
	9U, // SFX_TIME_OUT
	9U, // SFX_TIME_PICKUP
	8U  // SFX_WARP_START
};

void snd_init() {
	snd_active1 = snd_active4 = 0U;
	snd_priority1 = snd_priority4 = 0U;
}

void wait_sound_done() {
	while(snd_active1 || snd_active4) {
		snd_update();
		wait_vbl_done();
	}
}

void playSound(UBYTE id) {
	UBYTE *data, *data1, *data4;
	UBYTE prio;

	disable_interrupts();
	SWITCH_ROM_MBC1(SOUND_BANK);

	switch(id) {
		case SFX_BUMP:
			data = &sfx_bump_data; break;
		case SFX_BUMP_ALIEN:
			data = &sfx_bump_alien_data; break;
		case SFX_CAT_DISABLE:
			data = &sfx_cat_disable_data; break;
		case SFX_CAT_ENABLE:
			data = &sfx_cat_enable_data; break;
		case SFX_DASH:
			data = &sfx_dash_data; break;
		case SFX_HIGHSCORE_SWITCH:
			data = &sfx_highscore_switch_data; break;
		case SFX_JETPACK:
			data = &sfx_jetpack_data; break;
		case SFX_MENU_CANCEL:
			data = &sfx_menu_cancel_data; break;
		case SFX_MENU_CONFIRM:
			data = &sfx_menu_confirm_data; break;
		case SFX_MENU_LOCKED:
			data = &sfx_menu_locked_data; break;
		case SFX_MENU_SWITCH:
			data = &sfx_menu_switch_data; break;
		case SFX_PLAYER_DIE:
			data = &sfx_player_die_data; break;
		case SFX_STOMP_ALIEN:
			data = &sfx_stomp_alien_data; break;
		case SFX_STOMP_BAT:
			data = &sfx_stomp_bat_data; break;
		case SFX_STOMP_BIRD:
			data = &sfx_stomp_bird_data; break;
		case SFX_STOMP_GHOST:
			data = &sfx_stomp_ghost_data; break;
		case SFX_TIME_LOW:
			data = &sfx_time_low_data; break;
		case SFX_TIME_OUT:
			data = &sfx_time_out_data; break;
		case SFX_TIME_PICKUP:
			data = &sfx_time_pickup_data; break;
		case SFX_WARP_START:
			data = &sfx_warp_start_data; break;
	}

	data1 = data + ((UWORD*)data)[CHN1_OFFSET];
	data4 = data + ((UWORD*)data)[CHN4_OFFSET];
	prio = sfx_priority[id];

	if(*data1 != T_EOF && prio >= snd_priority1) {
		mus_disable1();
		snd_data1 = data1;
		snd_active1 = 1U;
		snd_priority1 = prio;

		snd_wait1 = 0U;
		snd_octave1 = 4U;
		snd_length1 = 48U;
		snd_volume1 = 0xE0U;
		snd_env1 = 0U;
		snd_slide1 = 0U;
		snd_vib_speed1 = 0U;
		snd_po1 = 128U;
		NR51_REG |= 0x11U;
	}
	if(*data4 != T_EOF && prio >= snd_priority4) {
		mus_disable4();
		snd_data4 = data4;
		snd_active4 = 1U;
		snd_priority4 = prio;

		snd_wait4 = 0U;
		snd_octave4 = 4U;
		snd_length4 = 48U;
		snd_volume4 = 0xE0U;
		snd_env4 = 0U;
		snd_slide4 = 0U;
		snd_noise_step = 0U;
		NR51_REG |= 0x88U;
	}

	SWITCH_ROM_MBC1(game_bank);
	enable_interrupts();
}

void snd_update() {
	disable_interrupts();
	SWITCH_ROM_MBC1(SOUND_BANK);
	if(snd_active1) snd_update1();
	if(snd_active4) snd_update4();
	SWITCH_ROM_MBC1(game_bank);
	enable_interrupts();
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
		if(note >= MUS_FIRST_NOTE) {
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
					snd_target1 = freq[(snd_octave1 << 4) + note - MUS_FIRST_NOTE] + snd_po1 - 128U;
				} else {
					snd_freq1 = freq[(snd_octave1 << 4) + note - MUS_FIRST_NOTE] + snd_po1 - 128U;
				}
				NR12_REG = snd_volume1 | snd_env1;
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
				NR12_REG = snd_volume1 | snd_env1;
				break;
			case T_ENV:
				snd_env1 = *snd_data1++;
				NR12_REG = snd_volume1 | snd_env1;
				break;
			case T_WAVEDUTY:
				NR11_REG = (*snd_data1++) << 5;
				break;
			case T_PAN:
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
			case T_VIBRATO_DELAY:
				break;
			case T_REP_START:
				break;
			case T_REP_END:
				break;
			case T_LOOP:
				break;
			case T_PITCH_OFFSET:
				snd_po1 = *snd_data1++;
				break;
			case T_TEMPO:
				break;
			case T_NOISE_STEP:
				break;
			case T_WAVE:
				break;
			case T_EOF:
				snd_active1 = 0U;
				snd_priority1 = 0U;
				mus_restore1();
				return;
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
		if(note >= MUS_FIRST_NOTE) {
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
					snd_target4 = noise_freq[((snd_octave4-MUS_NOISE_FIRST_OCTAVE) << 4) + note - MUS_FIRST_NOTE];
				} else {
					snd_freq4 = noise_freq[((snd_octave4-MUS_NOISE_FIRST_OCTAVE) << 4) + note - MUS_FIRST_NOTE];
				}
				NR42_REG = snd_volume4 | snd_env4;
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
				NR42_REG = snd_volume4 | snd_env4;
				break;
			case T_ENV:
				snd_env4 = *snd_data4++;
				NR42_REG = snd_volume4 | snd_env4;
				break;
			case T_WAVEDUTY:
				break;
			case T_PAN:
				break;
			case T_PORTAMENTO:
				snd_slide4 = *snd_data4++;
				break;
			case T_VIBRATO:
				break;
			case T_VIBRATO_DELAY:
				break;
			case T_REP_START:
				break;
			case T_REP_END:
				break;
			case T_LOOP:
				break;
			case T_PITCH_OFFSET:
				break;
			case T_TEMPO:
				break;
			case T_NOISE_STEP:
				snd_noise_step = (*snd_data4++) << 3;
				break;
			case T_WAVE:
				break;
			case T_EOF:
				snd_active4 = 0U;
				snd_priority4 = 0U;
				mus_restore4();
				return;
		}
	}
}
