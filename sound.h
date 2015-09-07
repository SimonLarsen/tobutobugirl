#ifndef SOUND_H
#define SOUND_H

#define SND_TYPE_SQUARE 1U
#define SND_TYPE_NOISE  2U

#define SND_CONFIRM 0U
#define SND_CANCEL  1U
#define SND_CLICK   2U
#define SND_JUMP    3U
#define SND_DASH    4U
#define SND_STOMP   5U

void snd_init();
void snd_play(UBYTE id);
void snd_update();

#endif
