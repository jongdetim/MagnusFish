#include "MagnusFish.hpp"
#include "Board.hpp"

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
