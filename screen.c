#include <gb/gb.h>
#include "screen.h"

void setScreenTiles(UBYTE *data, UBYTE data_length, UBYTE *tiles) {
	set_bkg_data(0U, data_length, data);
	set_bkg_tiles(0U, 0U, 20U, 18U, tiles);
}
