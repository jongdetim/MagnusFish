#include "Board.hpp"

void	Board::getPieceIndexes(u64 bitboard)
{
	int index = 0;

	for (int i = 0; i < 64; i++)
	{
		if (((bitboard >> i) & 1UL) == 1)
		{
			pieceSQs[index] = i;
			index++;
		}
	}
	while (index < 10)
	{
		pieceSQs[index] = 0;
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

char	Board::getPiece(int square)
{
	if (((pawns >> square) & 1) == 1)
	{
		return ('P');
	}
	else if (((knights >> square) & 1) == 1)
	{
		return ('N');
	}
	else if (((bishops >> square) & 1) == 1)
	{
		return ('B');
	}
	else if (((rooks >> square) & 1) == 1)
	{
		return ('R');
	}
	else if (((queens >> square) & 1) == 1)
	{
		return ('Q');
	}
	else if (((kings >> square) & 1) == 1)
	{
		return ('K');
	}
	return ('.');
}

bool	Board::compareSquares(int square)
{
	for (int i = 0; pieceSQs[i] != 0 && i < 10; i++)
	{
		if (square == pieceSQs[i])
		{
			return (true);
		}
	}
}