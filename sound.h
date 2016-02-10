#ifndef SOUND_H
#define SOUND_H

void snd_init();
void playSound(UBYTE id);
void snd_update();
void snd_update1();
void snd_update4();

#define SOUND_BANK 10U

#define SFX_BUMP              0U
#define SFX_BUMP_ALIEN        1U
#define SFX_DASH              2U
#define SFX_HIGHSCORE_SWITCH  3U
#define SFX_MENU_CANCEL       4U
#define SFX_MENU_CONFIRM      5U
#define SFX_MENU_SWITCH       6U
#define SFX_PLAYER_DIE        7U
#define SFX_STOMP             8U
#define SFX_STOMP_BAT         9U
#define SFX_TIME_LOW         10U
#define SFX_TIME_PICKUP      11U

#endif
