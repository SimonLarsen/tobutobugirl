#ifndef ZOOM_CIRCLES_MAP_H
#define ZOOM_CIRCLES_MAP_H

#define zoom_circles_data_length 9
const unsigned char zoom_circles_data[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,  24,   0,  24,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,  24,   0,  60,   0, 126,   0, 126,   0,  60,   0,  24,   0,   0,   0, 
	 60,   0, 126,   0, 255,   0, 255,   0, 255,   0, 255,   0, 126,   0,  60,   0, 
	255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 
	255,   0, 255,   0, 255,   0, 231,   0, 231,   0, 255,   0, 255,   0, 255,   0, 
	255,   0, 231,   0, 195,   0, 129,   0, 129,   0, 195,   0, 231,   0, 255,   0, 
	195,   0, 129,   0,   0,   0,   0,   0,   0,   0,   0,   0, 129,   0, 195,   0, 
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

#define zoom_circles_tiles_width 1
#define zoom_circles_tiles_height 9
#define zoom_circles_offset 0
const unsigned char zoom_circles_tiles[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8
};

#endif
