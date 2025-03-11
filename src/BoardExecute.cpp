#include "Board.hpp"

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

static int	getLowestBit(u64 bitboard)
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

static int	getHighestBit(u64 bitboard)
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

void	Board::makeMove(Move move)
{

}

bool	Board::orthogonallyInCheck(int king)
{
	u64	orthogonalReach = 0;
	int	square;

	/*	Search north	*/
	searchOrthogonalDirection(NORTH, king, orthogonalReach);
	square = getLowestBit(orthogonalReach);
	getPieceIndexes(rooks & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	orthogonalReach = 0;

	/*	Search south	*/
	searchOrthogonalDirection(SOUTH, king, orthogonalReach);
	square = getHighestBit(orthogonalReach);
	getPieceIndexes(rooks & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	orthogonalReach = 0;

	/*	Search east	*/
	searchOrthogonalDirection(EAST, king, orthogonalReach);
	square = getEasternBit(orthogonalReach);
	getPieceIndexes(rooks & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	orthogonalReach = 0;

	/*	Search west	*/
	searchOrthogonalDirection(WEST, king, orthogonalReach);
	square = getWesternBit(orthogonalReach);
	getPieceIndexes(rooks & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
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

	searchDiagonalDirection(NORTHWEST, king, diagonalReach);
	square = getLowestBit(diagonalReach);
	getPieceIndexes(bishops & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	diagonalReach = 0;

	/*	Search northeast	*/	
	searchDiagonalDirection(NORTHEAST, king, diagonalReach);
	square = getEasternBit(diagonalReach);
	getPieceIndexes(bishops & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	diagonalReach = 0;

	/*	Search southwest	*/
	searchDiagonalDirection(SOUTHWEST, king, diagonalReach);
	square = getWesternBit(diagonalReach);
	getPieceIndexes(bishops & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	diagonalReach = 0;

	/*	Search southeast	*/
	searchDiagonalDirection(SOUTHEAST, king, diagonalReach);
	square = getHighestBit(diagonalReach);
	getPieceIndexes(bishops & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	if (compareSquares(square) == true)
	{
		return (true);
	}
	return (false);
}

bool	Board::inKnightCheck(int king)
{
	getPieceIndexes(knights & pieces[!sideToMove]);
	for (int moves = 0; moves < 8; moves++)
	{
		if (compareSquares(king + knightMoves[moves]) == true)
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