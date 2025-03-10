#include "Board.hpp"

u64	generateRookMoves(u16 rookPosition)
{
	u64	moves = 0;
	u16 row = rookPosition / 8;
	u16 file = rookPosition % 8;

	for (u16 i = 0; i < 8; i++)
	{
		moves |= 1UL << (file + 8 * i);
		moves |= 1UL << (row * 8 + i);
	}
	moves ^= 1UL << rookPosition;
	printBitboard(moves);
	return (moves);
}