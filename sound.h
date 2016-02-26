#ifndef SOUND_H
#define SOUND_H

void snd_init();
void wait_sound_done();
void playSound(UBYTE id);
void snd_update();
void snd_update1();
void snd_update4();

#define SOUND_BANK 10U

#define SFX_BUMP 0U
#define SFX_BUMP_ALIEN 1U
#define SFX_CAT_DISABLE 2U
#define SFX_CAT_ENABLE 3U
#define SFX_DASH 4U
#define SFX_HIGHSCORE_SWITCH 5U
#define SFX_JETPACK 6U
#define SFX_MENU_CANCEL 7U
#define SFX_MENU_CONFIRM 8U
#define SFX_MENU_LOCKED 9U
#define SFX_MENU_SWITCH 10U
#define SFX_PLAYER_DIE 11U
#define SFX_STOMP_ALIEN 12U
#define SFX_STOMP_BAT 13U
#define SFX_STOMP_BIRD 14U
#define SFX_STOMP_GHOST 15U
#define SFX_TIME_LOW 16U
#define SFX_TIME_PICKUP 17U
#define SFX_WARP_END 18U
#define SFX_WARP_START 19U

#endif
