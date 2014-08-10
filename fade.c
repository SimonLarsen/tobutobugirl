#include <gb/gb.h>
#include "binconst.h"
#include "fade.h"

const UINT8 fadePals[] = {
	B8(11100100), B8(10010000),
	B8(01000000), B8(00000000),
};

void fadeToWhite() {
	UINT8 i;
	for(i = 1; i != 4; ++i) {
		BGP_REG = fadePals[i];
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
	}
}

void fadeFromWhite() {
	UINT8 i;

	for(i = 3; i != 0; --i) {
		BGP_REG = fadePals[i];
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
		wait_vbl_done(); wait_vbl_done();
	}

	BGP_REG = fadePals[0];
}
