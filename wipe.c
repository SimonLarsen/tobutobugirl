#include <gb/gb.h>
#include "main.h"
#include "wipe.h"
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "sound.h"

#include "data/bg/circles.h"
#include "data/bg/wipe.h"
#include "data/sprite/wipe_marker.h"

void initWipe() {
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_bkg_data(0U, circles_data_length, circles_data);
	set_bkg_data_rle(wipe_offset, wipe_data_length, wipe_data);
	set_sprite_data(0U, wipe_marker_data_length, wipe_marker_data);

	set_bkg_tiles_rle(0U, 0U, wipe_tiles_width, wipe_tiles_height, wipe_tiles);

	ticks = 0U;
	circle_index = 0U;
	sub_selection = 0U;
	player_skin = 1U;

	OBP0_REG = 0xD0U;
	BGP_REG = 0xE4U;
	SPRITES_8x16;

	clearSprites();

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterWipe() {
	initWipe();

	fadeFromWhite(6U);

	while(1U) {
		updateJoystate();
		
		ticks++;
		if((ticks & 3U) == 3U) {
			circle_index = (circle_index+1U) & 7U;
			set_bkg_data(13U, 1U, &circles_data[(circle_index << 4)]);
		}

		if(CLICKED(J_UP) && sub_selection == 1U) {
			sub_selection = 0U;
			playSound(SFX_MENU_SWITCH);
		}
		if(CLICKED(J_DOWN) && sub_selection == 0U) {
			sub_selection = 1U;
			playSound(SFX_MENU_SWITCH);
		}

		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
			playSound(SFX_MENU_CANCEL);
			break;
		}
		if(CLICKED(J_A)) {
			if(sub_selection == 1U) {
				initRAM(1U);
			}
			playSound(SFX_MENU_CONFIRM);
			gamestate = GAMESTATE_TITLE;
			break;
		}

		setSprite(67U, 123U+(sub_selection << 3), 0U, OBJ_PAL0);
		setSprite(75U, 123U+(sub_selection << 3), 2U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(6U);
	wait_sound_done();
}
