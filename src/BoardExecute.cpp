#include "Board.hpp"

void	Board::makeMove(const Move& move)
{
	sideToMove = !sideToMove;
	if (move.capturedPiece != NONE)
	{
		pieces[!sideToMove] &= ~(1UL << move.newSquare);
	}
}

void	Board::undoMove(const Move& move)
{
	sideToMove = !sideToMove;
}

void	Board::verifyAndAddMove(const Move& move)
{
	makeMove(move);
	getPieceIndexes(pieces[KING] & pieces[sideToMove]);
	if (inCheck(pieceSQs[0]) == false)
	{
		moveList.push_back(move);
	}
}