#include <gb/gb.h>
#include "defines.h"
#include "fade.h"
#include "gamestate.h"
#include "intro.h"

void initIntro() {

}

void enterIntro() {
	initIntro();

	fadeFromWhite(10U);

	while(1U) {
		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(10U);

	stopMusic();
}
