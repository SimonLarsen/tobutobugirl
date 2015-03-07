#include <gb/gb.h>

/*
Memory:
	00-09	Level 1 times
	10-19	Level 2 times
	20-29	Level 3 times
	   30	Unused
	   31	First boot flag
 */
UBYTE ram_data[32];
