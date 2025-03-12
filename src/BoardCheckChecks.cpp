#include "Board.hpp"

static bool	isOnOuterFile(int square)
{
	return (((1UL << square) & A_FILE) != 0 || ((1UL << square) & H_FILE) != 0);
}

static int	getEasternBit(u64 bitboard)
{
	int res = 0;

	for (int i = 0; i < 64; i++)
	{
		if (((bitboard >> i) & 1UL) == 1 && i % 8 > res % 8)
		{
			res = i;
		}
	}
	return (res);
}

static int	getWesternBit(u64 bitboard)
{
	int res = 0;

	for (int i = 0; i < 64; i++)
	{
		if (((bitboard >> i) & 1UL) == 1 && i % 8 < res % 8)
		{
			res = i;
		}
	}
	return (res);
}

static int	getNorthernBit(u64 bitboard)
{
	for (int i = 0; i < 64; i++)
	{
		if (((bitboard >> i) & 1UL) == 1)
		{
			return (i);
		}
	}
	return (0);
}

static int	getSouthernBit(u64 bitboard)
{
	for (int i = 63; i >= 0; i--)
	{
		if (((bitboard >> i) & 1UL) == 1)
		{
			return (i);
		}
	}
	return (0);
}

void	Board::searchDiagonally(int direction, int square, u64& moveOptions)
{
	while (true)
	{
		if (isOnOuterFile(square) == true)
		{
			square += direction;
			if (isOnOuterFile(square) == true)
			{
				break;
			}
		}
		else
		{
			square += direction;
		}
		if (square < 0 || square > 63 || ((pieces[sideToMove] >> square) & 1UL) == 1)
		{
			break;
		}
		moveOptions |= 1UL << square;
		if (((pieces[!sideToMove] >> square) & 1UL) == 1)
		{
			break;
		}
	}
}

void	Board::searchOrthogonally(int direction, int square, u64& moveOptions)
{
	int test;

	while (square >= 0 && square < 64)
	{
		test = square % 8 + direction % 8;
		if (test < 0 || test > 7)
		{
			break;
		}
		square += direction;
		if (((pieces[sideToMove] >> square) & 1UL) == 1)
		{
			break;
		}
		moveOptions |= 1UL << square;
		if (((pieces[!sideToMove] >> square) & 1UL) == 1)
		{
			break;
		}
	}
}

bool	Board::orthogonallyInCheck(int king)
{
	u64	orthogonalReach = 0;
	int	square;

	/*	Search north	*/
	searchOrthogonally(NORTH, king, orthogonalReach);
	square = getNorthernBit(orthogonalReach);
	getPieceIndexes(pieces[ROOK] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	orthogonalReach = 0;

	/*	Search south	*/
	searchOrthogonally(SOUTH, king, orthogonalReach);
	square = getSouthernBit(orthogonalReach);
	getPieceIndexes(pieces[ROOK] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	orthogonalReach = 0;

	/*	Search east	*/
	searchOrthogonally(EAST, king, orthogonalReach);
	square = getEasternBit(orthogonalReach);
	getPieceIndexes(pieces[ROOK] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	orthogonalReach = 0;

	/*	Search west	*/
	searchOrthogonally(WEST, king, orthogonalReach);
	square = getWesternBit(orthogonalReach);
	getPieceIndexes(pieces[ROOK] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	return (false);
}

bool	Board::diagonallyInCheck(int king)
{
	u64 diagonalReach = 0;
	int square;

	/*	Search northwest	*/

	searchDiagonally(NORTHWEST, king, diagonalReach);
	square = getNorthernBit(diagonalReach);
	getPieceIndexes(pieces[BISHOP] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	diagonalReach = 0;

	/*	Search northeast	*/	
	searchDiagonally(NORTHEAST, king, diagonalReach);
	square = getEasternBit(diagonalReach);
	getPieceIndexes(pieces[BISHOP] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	diagonalReach = 0;

	/*	Search southwest	*/
	searchDiagonally(SOUTHWEST, king, diagonalReach);
	square = getWesternBit(diagonalReach);
	getPieceIndexes(pieces[BISHOP] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	diagonalReach = 0;

	/*	Search southeast	*/
	searchDiagonally(SOUTHEAST, king, diagonalReach);
	square = getSouthernBit(diagonalReach);
	getPieceIndexes(pieces[BISHOP] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(pieces[QUEEN] & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	return (false);
}

bool	Board::inKnightCheck(int king)
{
	getPieceIndexes(pieces[KNIGHT] & pieces[!sideToMove]);
	for (int moves = 0; moves < 8; moves++)
	{
		if (compareSquares(king - knightMoves[moves]) == true)
		{
			return (true);
		}
	}
	return (false);
}

bool	Board::inPawnCheck(int kingIndex)
{
	getPieceIndexes(pieces[PAWN] & pieces[!sideToMove]);
	if (sideToMove == WHITE)
	{
		if (compareSquares(kingIndex + NORTHWEST) == true || compareSquares(kingIndex + NORTHEAST) == true)
		{
			return (true);
		}
	}
	else
	{
		if (compareSquares(kingIndex + SOUTHEAST) == true || compareSquares(kingIndex + SOUTHWEST) == true)
		{
			return (true);
		}
	}
	return (false);
}

bool	Board::inCheck(int kingIndex)
{
	if (inKnightCheck(kingIndex) == true)
	{
		return (true);
	}
	if (diagonallyInCheck(kingIndex) == true)
	{
		return (true);
	}
	if (orthogonallyInCheck(kingIndex) == true)
	{
		return (true);
	}
	if (inPawnCheck(kingIndex) == true)
	{
		return (true);
	}
	return (false);
}