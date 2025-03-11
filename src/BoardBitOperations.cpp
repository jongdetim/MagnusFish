#include "Board.hpp"

void	Board::getPieceIndexes(u64 bitboard)
{
	int index = 0;

	for (int i = 0; i < 64; i++)
	{
		if (((bitboard >> i) & 1UL) == 1)
		{
			pieceLocations[index] = i;
			index++;
		}
	}
	while (index < 10)
	{
		pieceLocations[index] = 0;
		index++;
	}
}

void	printBitboard(u64 bitboard)
{
	for (int x = 1; x <= 64; x++)
	{
		std::cout << ((bitboard >> (x - 1)) & 1);
		if (x % 8 == 0)
		{
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}
