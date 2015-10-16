#ifndef GAMESTATE_H
#define GAMESTATE_H

void clearSprites();
void updateJoystate();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void setIngameBackground(UBYTE level);
void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data);
void set_bkg_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles);
void setGameBank(UBYTE i);
void setMusicBank(UBYTE i);
void playMusic(UBYTE *data);
void updateMusic();
void stopMusic();

extern UBYTE ticks, paused;
extern UBYTE gamestate, scene_state, level, selection;
extern UBYTE joystate, oldjoystate;
extern UBYTE elapsed_time, remaining_time, kills;
extern UBYTE last_highscore_level, last_highscore_slot;
extern UBYTE game_bank, music_bank;
extern UBYTE levels_completed;
extern UBYTE unlocked_bits;
extern UBYTE scroll_x, scroll_y;
extern UBYTE player_x, player_y;
extern UBYTE cat_x, cat_y;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#define GAMESTATE_LOGOS     0U
#define GAMESTATE_INTRO     1U
#define GAMESTATE_TITLE     2U
#define GAMESTATE_SELECT    3U
#define GAMESTATE_INGAME    4U
#define GAMESTATE_WINSCREEN 5U
#define GAMESTATE_HIGHSCORE 6U
#define GAMESTATE_JUKEBOX   7U
#define GAMESTATE_UNLOCKED  8U
#define GAMESTATE_ENDING    9U

#define UNLOCKED_CLOUDS 1U
#define UNLOCKED_SPACE  2U
#define UNLOCKED_MUSIC  4U
#define UNLOCKED_DREAM  8U

#define TIME_BONUS (2U*level_max_time[level-1U])
#define KILL_BONUS (kills)
#define TOTAL_SCORE (TIME_BONUS + KILL_BONUS)

extern const UBYTE level_names[7][6];
extern const UBYTE level_max_time[4];

#endif
