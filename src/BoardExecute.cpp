#include "Board.hpp"

void	Board::uncastle(int castleDirection)
{
	switch (castleDirection)
	{
		case whiteKingSide:
			indexBoard[H1] = indexBoard[F1];
			indexBoard[F1] = NONE;
			indexBoard[E1] = indexBoard[G1];
			indexBoard[G1] = NONE;
			pieces[KING] ^= 1UL << E1;
			pieces[KING] ^= 1UL << G1;
			pieces[ROOK] ^= 1UL << H1;
			pieces[ROOK] ^= 1UL << F1;
			break;

		case whiteQueenSide:
			indexBoard[A1] = indexBoard[D1];
			indexBoard[D1] = NONE;
			indexBoard[E1] = indexBoard[C1];
			indexBoard[C1] = NONE;
			pieces[KING] ^= 1UL << E1;
			pieces[KING] ^= 1UL << C1;
			pieces[ROOK] ^= 1UL << A1;
			pieces[ROOK] ^= 1UL << D1;
			break;

		case blackKingSide:
			indexBoard[H8] = indexBoard[F8];
			indexBoard[F8] = NONE;
			indexBoard[E8] = indexBoard[G8];
			indexBoard[G8] = NONE;
			pieces[KING] ^= 1UL << E8;
			pieces[KING] ^= 1UL << G8;
			pieces[ROOK] ^= 1UL << H8;
			pieces[ROOK] ^= 1UL << F8;
			break;

		case blackQueenSide:
			indexBoard[A8] = indexBoard[D8];
			indexBoard[D8] = NONE;
			indexBoard[E8] = indexBoard[C8];
			indexBoard[C8] = NONE;
			pieces[KING] ^= 1UL << E8;
			pieces[KING] ^= 1UL << C8;
			pieces[ROOK] ^= 1UL << A8;
			pieces[ROOK] ^= 1UL << D8;
			break;
	}
	castlingRights ^= castleDirection;
}

void	Board::castle(int castleDirection)
{
	switch (castleDirection)
	{
		case whiteKingSide:
			indexBoard[F1] = indexBoard[H1];
			indexBoard[H1] = NONE;
			indexBoard[G1] = indexBoard[E1];
			indexBoard[E1] = NONE;
			pieces[KING] ^= 1UL << E1;
			pieces[KING] |= 1UL << G1;
			pieces[ROOK] ^= 1UL << H1;
			pieces[ROOK] |= 1UL << F1;
			break;

		case whiteQueenSide:
			indexBoard[D1] = indexBoard[A1];
			indexBoard[A1] = NONE;
			indexBoard[C1] = indexBoard[E1];
			indexBoard[E1] = NONE;
			pieces[KING] ^= 1UL << E1;
			pieces[KING] |= 1UL << C1;
			pieces[ROOK] ^= 1UL << A1;
			pieces[ROOK] |= 1UL << D1;
			break;

		case blackKingSide:
			indexBoard[F8] = indexBoard[H8];
			indexBoard[H8] = NONE;
			indexBoard[G8] = indexBoard[E8];
			indexBoard[E8] = NONE;
			pieces[KING] ^= 1UL << E8;
			pieces[KING] |= 1UL << G8;
			pieces[ROOK] ^= 1UL << H8;
			pieces[ROOK] |= 1UL << F8;
			break;

		case blackQueenSide:
			indexBoard[D8] = indexBoard[A8];
			indexBoard[A8] = NONE;
			indexBoard[C8] = indexBoard[E8];
			indexBoard[E8] = NONE;
			pieces[KING] ^= 1UL << E8;
			pieces[KING] |= 1UL << C8;
			pieces[ROOK] ^= 1UL << A8;
			pieces[ROOK] |= 1UL << D8;
			break;
		
		default:
			break;
	}
	castlingRights ^= castleDirection;
}

void	Board::makeMove(const Move& move)
{
	sideToMove = !sideToMove;
	if (move.castle != 0)
	{
		castle(move.castle);
		return ;
	}
	indexBoard[move.newSquare] = indexBoard[move.startingSquare];
	indexBoard[move.startingSquare] = NONE;
	for (int i = 0; i < 9; i++)
	{
		if (((pieces[i] >> move.startingSquare) & 1UL) == 1)
		{
			pieces[i] ^= 1UL << move.startingSquare;
			pieces[i] |= 1UL << move.newSquare;
		}
	}
	halfMoveClock++;
	if (move.capturedPiece != NONE)
	{
		pieces[!sideToMove] ^= 1UL << move.newSquare;
		pieces[move.capturedPiece] ^= 1UL << move.newSquare;
		halfMoveClock = 0;
	}
	if (sideToMove == WHITE)
	{
		fullMoveCount++;
	}
}

void	Board::undoMove()
{
	pieces = position.pieces;
	indexBoard = position.indexBoard;
	sideToMove = position.sideToMove;
	enPassentSquare = position.enPassentSquare;
	castlingRights = position.castlingRights;
	halfMoveClock = position.halfMoveClock;
	fullMoveCount = position.fullMoveCount;
}

void	Board::verifyAndAddMove(const Move& move)
{
	makeMove(move);
	getPieceIndexes(pieces[KING] & pieces[sideToMove]);
	if (inCheck(pieceSQs[0]) == false)
	{
		moveList.push_back(move);
		std::cout << "From square: " << move.startingSquare << " To square: " << move.newSquare << std::endl;
		std::cout << "Captured piece: " << move.capturedPiece << std::endl;
	}
	undoMove();
}