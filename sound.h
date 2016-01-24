#ifndef SOUND_H
#define SOUND_H

void snd_init();
void playSound(UBYTE id);
void snd_update();
void snd_update1();
void snd_update4();

#define SFX_BLIP 1U
#define SFX_DASH 2U

#endif
