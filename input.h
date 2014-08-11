#ifndef INPUT_H
#define INPUT_H

void updateJoystate();

extern UBYTE joystate, oldjoystate;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define HELD(x) (joystate & x)

#endif
