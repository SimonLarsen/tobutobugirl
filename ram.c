#include <gb/gb.h>

/*
Memory:
	2 bytes per record. (seconds, score)
	10 bytes pr. level

	00-09	Level 1
	16-25	Level 2
	32-41	Level 3
	48-57   Level 4

	64U     Show dash counter option
	65U     Reverse keys option

	120-127 TOBUTOBU signature
 */

UBYTE ram_data[128];
