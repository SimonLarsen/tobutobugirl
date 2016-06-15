#include "cos.h"

const UBYTE cos32_64[64] = {
	0U, 0U, 0U, 1U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 11U, 13U, 14U, 16U,
	18U, 19U, 21U, 22U, 24U, 25U, 26U, 27U, 28U, 29U, 30U, 31U, 31U, 32U, 32U,
	32U, 32U, 32U, 31U, 31U, 30U, 29U, 28U, 27U, 26U, 25U, 24U, 22U, 21U, 19U,
	18U, 16U, 14U, 13U, 11U, 10U, 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 1U, 0U, 0U
};

const UBYTE cos16_32[32] = {
	0U, 0U, 1U, 1U, 2U, 4U, 5U, 6U, 8U, 10U,
	11U, 12U, 14U, 15U, 15U, 16U, 16U, 16U,
	15U, 15U, 14U, 12U, 11U, 10U, 8U,
	6U, 5U, 4U, 2U, 1U, 1U, 0U
};

const UBYTE cos4_8[8] = {
	0U, 0U, 1U, 2U, 3U, 3U, 2U, 1U
};

const UBYTE cos4_16[16] = {
	4U, 4U, 3U, 3U, 2U, 1U, 1U, 0U, 0U, 0U, 1U, 1U, 2U, 3U, 3U, 4U
};

const UBYTE cos8_16[16] = {
	8U, 8U, 7U, 6U, 4U, 2U, 1U, 0U, 0U, 0U, 1U, 2U, 4U, 6U, 7U, 8U
};
