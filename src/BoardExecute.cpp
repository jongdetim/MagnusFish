#include "Board.hpp"

void	Board::makeMove(Move move)
{

}

bool	Board::seesBishop(int king, int bishop)
{

	return (false);
}

bool	Board::seesKnight(int king, int knight)
{
	for (int i = 0; i < 8; i++)
	{
		if (king - knight == knightMoves[i])
		{
			return (true);
		}
	}
	return (false);
}

bool	Board::inCheck()
{
	int i;

	getPieceIndexes(kings & pieces[sideToMove]);
	int kingIndex = pieceLocations[0];
	getPieceIndexes(knights & pieces[!sideToMove]);
	for (i = 0; pieceLocations[i] != 0 && i < 10; i++)
	{
		if (seesKnight(kingIndex, pieceLocations[i]) == true)
		{
			return (true);
		}
	}
	getPieceIndexes(bishops & pieces[!sideToMove]);
	for (i = 0; pieceLocations[i] != 0 && i < 10; i++)
	{
		if (seesBishop(kingIndex, pieceLocations[i]) == true)
		{
			return (true);
		}
	}
	getPieceIndexes(rooks & pieces[!sideToMove]);
	for (i = 0; pieceLocations[i] != 0 && i < 10; i++)
	{
		if (seesRook(kingIndex, pieceLocations[i]) == true)
		{
			return (true);
		}
	}
	getPieceIndexes(queens & pieces[!sideToMove]);
	for (i = 0; pieceLocations[i] != 0 && i < 10; i++)
	{
		if (seesQueen(kingIndex, pieceLocations[i]) == true)
		{
			return (true);
		}
	}
	getPieceIndexes(pawns & pieces[!sideToMove]);
	for (i = 0; pieceLocations[i] != 0 && i < 10; i++)
	{
		if (seesPawn(kingIndex, pieceLocations[i]) == true)
		{
			return (true);
		}
	}
	return (false);
}