#ifndef SOUND_H
#define SOUND_H

void snd_init();
void playSound(UBYTE id);
void snd_update();
void snd_update1();
void snd_update4();

#define SFX_BUMP              1U
#define SFX_BUMP_ALIEN        2U
#define SFX_DASH              3U
#define SFX_HIGHSCORE_SWITCH  4U
#define SFX_MENU_CANCEL       5U
#define SFX_MENU_CONFIRM      6U
#define SFX_MENU_SWITCH       7U
#define SFX_PLAYER_DIE        8U
#define SFX_STOMP             9U
#define SFX_STOMP_BAT        10U
#define SFX_TIME_LOW         11U
#define SFX_TIME_PICKUP      12U

#endif
