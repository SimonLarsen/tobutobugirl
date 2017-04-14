#ifndef GAMESTATE_H
#define GAMESTATE_H

void clearSprites();
void updateJoystate();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void setIngameBackground(UBYTE level);
void setWinscreenBackground(UBYTE level);
void setCloudAnimation(UBYTE skin);
void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data);
void set_bkg_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles);
void setGameBank(UBYTE i);
void setMusicBank(UBYTE i);
void playMusic(UBYTE *data);
void updateMusic();
void stopMusic();
UBYTE getRank(UBYTE score, UBYTE level);

extern UBYTE ticks, paused;
extern UBYTE gamestate, scene_state;
extern UBYTE level, levels_completed;
extern UBYTE selection, sub_selection;
extern UBYTE joystate, oldjoystate;
extern UBYTE next_sprite, sprites_used;
extern UBYTE elapsed_time, remaining_time, kills;
extern UBYTE last_highscore_level, last_highscore_slot;
extern UBYTE last_progress;
extern UBYTE game_bank, music_bank;
extern UBYTE ending_flags;
extern UBYTE unlocked_bits;
extern UBYTE scroll_x, scroll_y;
extern UBYTE player_x, player_y;
extern UBYTE player_xdir, player_ydir;
extern UBYTE player_xspeed, player_yspeed, player_bounce;
extern UBYTE cat_x, cat_y, cat_frame;
extern UBYTE arrow_offset1, arrow_offset2;
extern UBYTE player_skin;
extern UBYTE show_dashcounter;
extern UBYTE circle_index;

#define MAX_ENTITIES 10U

extern UBYTE entity_x[MAX_ENTITIES];
extern UBYTE entity_y[MAX_ENTITIES];
extern UBYTE entity_type[MAX_ENTITIES];
extern UBYTE entity_dir[MAX_ENTITIES];
extern UBYTE entity_sprites[10];

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#define GAMESTATE_LOGOS      0U
#define GAMESTATE_INTRO      1U
#define GAMESTATE_TITLE      2U
#define GAMESTATE_SELECT     3U
#define GAMESTATE_INGAME     4U
#define GAMESTATE_PAUSE      5U
#define GAMESTATE_WINSCREEN  6U
#define GAMESTATE_HIGHSCORE  7U
#define GAMESTATE_JUKEBOX    8U
#define GAMESTATE_UNLOCKED   9U
#define GAMESTATE_ENDING    10U
#define GAMESTATE_WIPE      11U

#define UNLOCKED_CLOUDS 1U
#define UNLOCKED_SPACE  2U
#define UNLOCKED_MUSIC  4U
#define UNLOCKED_DREAM  8U

#define TIME_BONUS (3U*(level_max_time[level-1U] - elapsed_time))
#define KILL_BONUS (kills)
#define TOTAL_SCORE (TIME_BONUS + KILL_BONUS)

#define ENDING_FLAG_ENDING_FINISHED 1U
#define ENDING_FLAG_FROM_DREAM      2U

#define GAME_BACKGROUNDS_BANK 7U
#define WINSCREEN_BACKGROUNDS_BANK 9U
#define CLOUD_ANIMATIONS_BANK 8U

extern const UBYTE level_names[7][6];
extern const UBYTE level_max_time[4];
extern const UBYTE rank_letters[4];

#endif
