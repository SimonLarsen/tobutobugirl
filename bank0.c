#include <gb/gb.h>

/*
Memory:
	00-06	Level 1 scores
	08-14	Level 2 scores
	16-23	Level 3 scores

	24-31	Unused

	32-43
	44-55	Level 1 times
	56-67	Level 2 times
	68-79	Level 3 times

	80-126	Unused

	127		First boot flag
 */

UBYTE ram_data[128];
