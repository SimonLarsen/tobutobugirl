#ifndef GAMESTATE_H
#define GAMESTATE_H

void clearSprites();
void updateJoystate();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data);
void set_bkg_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles);
void setGameBank(UBYTE i);
void setMusicBank(UBYTE i);
void playMusic(UBYTE *data);
void updateMusic();
void stopMusic();

extern UBYTE ticks;
extern UBYTE gamestate, level, selection;
extern UBYTE joystate, oldjoystate;
extern UBYTE elapsed_time, remaining_time, kills;
extern UBYTE last_highscore_level, last_highscore_slot;
extern UBYTE game_bank, music_bank;
extern UBYTE levels_completed;
extern UBYTE unlocked_bits;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#define GAMESTATE_INTRO     0U
#define GAMESTATE_TITLE     1U
#define GAMESTATE_SELECT    2U
#define GAMESTATE_INGAME    3U
#define GAMESTATE_WINSCREEN 4U
#define GAMESTATE_HIGHSCORE 5U
#define GAMESTATE_JUKEBOX   6U
#define GAMESTATE_UNLOCKED  7U

#define UNLOCKED_CLOUDS 1U
#define UNLOCKED_SPACE  2U
#define UNLOCKED_MUSIC  4U

extern const UBYTE level_names[4][6];

#endif
