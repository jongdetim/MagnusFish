#include "Board.hpp"

/*	Loops until the edge of the board or the first piece is in the way.
	If enemy piece, square is included.	*/


static bool	isOnOuterFile(int square)
{
	return (((1UL << square) & A_FILE) != 0 || ((1UL << square) & H_FILE) != 0);
}

void	Board::searchDiagonalDirection(int direction, int square)
{
	Move	move(square);

	while (true)
	{
		if (isOnOuterFile(square) == true && isOnOuterFile(square + direction) == true)
		{
			break;
		}
		square += direction;
		if (square < 0 || square > 63 || ((pieces[sideToMove] >> square) & 1UL) == 1)
		{
			break;
		}
		move.newSquare = square;
		if (((pieces[!sideToMove] >> square) & 1UL) == 1)
		{
			move.capturedPiece = indexBoard[square];
			verifyAndAddMove(move);
			break;
		}
		verifyAndAddMove(move);
		move.reset();
	}
}

void	Board::searchOrthogonalDirection(int direction, int square)
{
	int 	test;
	Move	move(square);

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
		move.newSquare = square;
		if (((pieces[!sideToMove] >> square) & 1UL) == 1)
		{
			move.capturedPiece = indexBoard[square];
			verifyAndAddMove(move);
			break;
		}
		verifyAndAddMove(move);
		move.reset();
	}
}

void	Board::generateKingMoves()
{
	constexpr std::array<int, 8>	kingMoves =
	{
		NORTHWEST,
		NORTH,
		NORTHEAST,
		WEST,
		EAST,
		SOUTHWEST,
		SOUTH,
		SOUTHEAST
	};
	getPieceIndexes(pieces[KING] & pieces[sideToMove]);

	Move	move(pieceSQs[0]);
	int		test, square;
	
	for (int i = 0; i < 8; i++)
	{
		test = move.startingSquare % 8 + kingMoves[i] % 8;
		if (test < 0 || test > 7)
		{
			continue;
		}
		square = move.startingSquare + kingMoves[i];
		if (square >= 0 && square < 64 && ((pieces[sideToMove] >> square) & 1UL) == 0)
		{
			if (((pieces[!sideToMove] >> square) & 1UL) == 1)
			{
				move.capturedPiece = indexBoard[square];
			}
			move.newSquare = square;
			verifyAndAddMove(move);
			move.reset();
		}
	}
}

void	Board::generateWhitePawnMoves()
{
	Move	move(0);

	getPieceIndexes(pieces[PAWN] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != 0 && index < 10; index++)
	{
		move.startingSquare = pieceSQs[index];
		move.newSquare = move.startingSquare + NORTH;

		if (((pieces[ALL] >> move.newSquare) & 1UL) == 0)
		{
			if ((move.newSquare & ROW_8) != 0)
			{
				move.promotesTo = QUEEN;
				verifyAndAddMove(move);
				move.promotesTo = ROOK;
				verifyAndAddMove(move);
				move.promotesTo = BISHOP;
				verifyAndAddMove(move);
				move.promotesTo = KNIGHT;
			}
			verifyAndAddMove(move);
		}
		if ((move.startingSquare & ROW_2) != 0 && ((pieces[ALL] >> (move.newSquare + NORTH)) & 1UL) == 0)
		{
			move.newSquare += NORTH;
			verifyAndAddMove(move);
		}
		if ((move.startingSquare & A_FILE) == 0)
		{
			move.newSquare = move.startingSquare + NORTHWEST;
			if (move.newSquare == enPassentSquare)
			{
				move.capturedPiece = PAWN;
				verifyAndAddMove(move);
			}
			else if (((pieces[BLACK] >> move.newSquare) & 1UL) == 1)
			{
				move.capturedPiece = indexBoard[move.newSquare];
				verifyAndAddMove(move);
			}
		}
		if ((pieceSQs[index] & H_FILE) == 0)
		{
			move.newSquare = move.startingSquare + NORTHEAST;
			if (move.newSquare == enPassentSquare)
			{
				move.capturedPiece = PAWN;
				verifyAndAddMove(move);
			}
			else if (((pieces[BLACK] >> move.newSquare) & 1UL) == 1)
			{
				move.capturedPiece = indexBoard[move.newSquare];
				verifyAndAddMove(move);
			}
		}
	}
}

void	Board::generateBlackPawnMoves()
{
	Move	move(0);

	getPieceIndexes(pieces[PAWN] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != 0 && index < 10; index++)
	{
		move.startingSquare = pieceSQs[index];
		move.newSquare = move.startingSquare + SOUTH;

		if (((pieces[ALL] >> move.newSquare) & 1UL) == 0)
		{
			if ((move.newSquare & ROW_1) != 0)
			{
				move.promotesTo = QUEEN;
				verifyAndAddMove(move);
				move.promotesTo = ROOK;
				verifyAndAddMove(move);
				move.promotesTo = BISHOP;
				verifyAndAddMove(move);
				move.promotesTo = KNIGHT;
			}
			verifyAndAddMove(move);
		}
		if ((move.startingSquare & ROW_7) != 0 && ((pieces[ALL] >> (move.newSquare + SOUTH)) & 1UL) == 0)
		{
			move.newSquare += SOUTH;
			verifyAndAddMove(move);
		}
		if ((move.startingSquare & A_FILE) == 0)
		{
			move.newSquare = move.startingSquare + SOUTHWEST;
			if (move.newSquare == enPassentSquare)
			{
				move.capturedPiece = PAWN;
				verifyAndAddMove(move);
			}
			else if (((pieces[BLACK] >> move.newSquare) & 1UL) == 1)
			{
				move.capturedPiece = indexBoard[move.newSquare];
				verifyAndAddMove(move);
			}
		}
		if ((pieceSQs[index] & H_FILE) == 0)
		{
			move.newSquare = move.startingSquare + SOUTHEAST;
			if (move.newSquare == enPassentSquare)
			{
				move.capturedPiece = PAWN;
				verifyAndAddMove(move);
			}
			else if (((pieces[BLACK] >> move.newSquare) & 1UL) == 1)
			{
				move.capturedPiece = indexBoard[move.newSquare];
				verifyAndAddMove(move);
			}
		}
	}
}

void	Board::generateKnightMoves()
{
	int square, test;
	Move move(0);

	getPieceIndexes(pieces[KNIGHT] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != 0 && index < 10; index++)
	{
		move.startingSquare = pieceSQs[index];
		for (int i = 0; i < 8; i++)
		{
			test = pieceSQs[index] % 8 + knightMoves[i] % 8;
			if (test < 0 || test > 7)
			{
				continue;
			}
			square = pieceSQs[index] + knightMoves[i];
			if (square >= 0 && square < 64 && ((pieces[sideToMove] >> square) & 1UL) == 0)
			{
				move.newSquare = square;
				if (((pieces[!sideToMove] >> square) & 1UL) == 1)
				{
					move.capturedPiece = indexBoard[square];
				}
				verifyAndAddMove(move);
				move.reset();
			}
		}
		move.reset();
	}
}

void	Board::generateQueenMoves()
{
	getPieceIndexes(pieces[QUEEN] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != 0 && index < 10; index++)
	{
		searchDiagonalDirection(NORTHWEST, pieceSQs[index]);
		searchDiagonalDirection(NORTHEAST, pieceSQs[index]);
		searchDiagonalDirection(SOUTHWEST, pieceSQs[index]);
		searchDiagonalDirection(SOUTHEAST, pieceSQs[index]);
		searchOrthogonalDirection(WEST, pieceSQs[index]);
		searchOrthogonalDirection(EAST, pieceSQs[index]);
		searchOrthogonalDirection(NORTH, pieceSQs[index]);
		searchOrthogonalDirection(SOUTH, pieceSQs[index]);
	}
}

void	Board::generateBishopMoves()
{
	getPieceIndexes(pieces[BISHOP] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != 0 && index < 10; index++)
	{
		searchDiagonalDirection(NORTHWEST, pieceSQs[index]);
		searchDiagonalDirection(NORTHEAST, pieceSQs[index]);
		searchDiagonalDirection(SOUTHWEST, pieceSQs[index]);
		searchDiagonalDirection(SOUTHEAST, pieceSQs[index]);
	}
}

void	Board::generateRookMoves()
{
	getPieceIndexes(pieces[ROOK] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != 0 && index < 10; index++)
	{
		searchOrthogonalDirection(WEST, pieceSQs[index]);
		searchOrthogonalDirection(EAST, pieceSQs[index]);
		searchOrthogonalDirection(NORTH, pieceSQs[index]);
		searchOrthogonalDirection(SOUTH, pieceSQs[index]);
	}
}

void	Board::generateCastlingMoves()
{
	Move	move(-1);

	if (sideToMove == WHITE)
	{
		if ((castlingRights & whiteKingSide) != 0)
		{
			if (inCheck(F1) == false && inCheck(G1) == false)
			{
				if (((pieces[ALL] >> F1) & 1UL) == 0 && ((pieces[ALL] >> G1) & 1UL) == 0)
				{
					move.castle = whiteKingSide;
					moveList.push_back(move);
				}
			}
		}
		if ((castlingRights & whiteQueenSide) != 0)
		{
			if (inCheck(D1) == false && inCheck(C1) == false)
			{
				if (((pieces[ALL] >> D1) & 1UL) == 0 && ((pieces[ALL] >> C1) & 1UL) == 0)
				{
					move.castle = whiteQueenSide;
					moveList.push_back(move);
				}
			}
		}
	}
	else
	{
		if ((castlingRights & blackKingSide) != 0)
		{
			if (inCheck(F8) == false && inCheck(G8) == false)
			{
				if (((pieces[ALL] >> F8) & 1UL) == 0 && ((pieces[ALL] >> G8) & 1UL) == 0)
				{
					move.castle = blackKingSide;
					moveList.push_back(move);
				}
			}
		}
		if ((castlingRights & blackQueenSide) != 0)
		{
			if (inCheck(D8) == false && inCheck(C8) == false)
			{
				if (((pieces[ALL] >> D8) & 1UL) == 0 && ((pieces[ALL] >> C8) & 1UL) == 0)
				{
					move.castle = blackQueenSide;
					moveList.push_back(move);
				}
			}
		}
	}
}


void	Board::generateMoves()
{
	getPieceIndexes(pieces[sideToMove]);
	bool kingIsChecked = inCheck(pieceSQs[0]);

	generateRookMoves();
	generateBishopMoves();
	generateKnightMoves();
	generateQueenMoves();
	generateKingMoves();
	if (sideToMove == WHITE)
	{
		generateWhitePawnMoves();
	}
	else
	{
		generateBlackPawnMoves();
	}
	if (kingIsChecked == false)
	{
		generateCastlingMoves();
	}
}