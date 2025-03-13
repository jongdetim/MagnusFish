#include "Board.hpp"

/*	Loops until the edge of the board or the first piece is in the way.
	If enemy piece, square is included.	*/


static bool	isOnOuterFile(int square)
{
	return (((1UL << square) & A_FILE) != 0 || ((1UL << square) & H_FILE) != 0);
}

void	Board::promote(Move& move)
{
	move.promotesTo = QUEEN;
	verifyAndAddMove(move);
	move.promotesTo = ROOK;
	verifyAndAddMove(move);
	move.promotesTo = BISHOP;
	verifyAndAddMove(move);
	move.promotesTo = KNIGHT;
	verifyAndAddMove(move);
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

	while (true)
	{
		test = square % 8 + direction % 8;
		if (test < 0 || test > 7)
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

	if (pieceSQs[0] == -1)
	{
		return;
	}
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

void	Board::pawnCapture(Move& move, int direction)
{
	constexpr std::array<u64, 2> promotionRanks = {ROW_8, ROW_1};

	move.newSquare = move.startingSquare + direction;
	if (move.newSquare == enPassentSquare)
	{
		move.capturedPiece = PAWN;
		verifyAndAddMove(move);
		return ;
	}
	if (((pieces[!sideToMove] >> move.newSquare) & 1UL) == 1)
	{
		move.capturedPiece = indexBoard[move.newSquare];
		if (((1UL << move.newSquare) & promotionRanks[sideToMove]) != 0)
		{
			promote(move);
		}
		else
		{
			verifyAndAddMove(move);
		}
	}
}

void	Board::generateWhitePawnMoves()
{
	Move	move(0);

	getPieceIndexes(pieces[PAWN] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != -1 && index < 10; index++)
	{
		for (int i = 0; i < 10; i++)
		{
			std::cout << pieceSQs[i] << std::endl;
		}
		move.startingSquare = pieceSQs[index];
		move.newSquare = move.startingSquare + NORTH;

		if (((pieces[ALL] >> move.newSquare) & 1UL) == 0)
		{
			if (((1UL << move.newSquare) & ROW_8) != 0)
			{
				promote(move);
			}
			else
			{
				verifyAndAddMove(move);
			}
		}
		if (((1UL << move.startingSquare) & ROW_2) != 0)
		{
			move.newSquare += NORTH;
			if (((pieces[ALL] >> move.newSquare) & 1UL) == 0)
			{
				verifyAndAddMove(move);
			}
		}
		if (((1UL << move.startingSquare) & A_FILE) == 0)
		{
			pawnCapture(move, NORTHWEST);
		}
		if (((1UL << move.startingSquare) & H_FILE) == 0)
		{
			pawnCapture(move, NORTHEAST);
		}
	}
}

void	Board::generateBlackPawnMoves()
{
	Move	move(0);

	getPieceIndexes(pieces[PAWN] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != -1 && index < 10; index++)
	{
		move.startingSquare = pieceSQs[index];
		move.newSquare = move.startingSquare + SOUTH;

		if (((pieces[ALL] >> move.newSquare) & 1UL) == 0)
		{
			if (((1UL << move.newSquare) & ROW_1) != 0)
			{
				promote(move);
			}
			else
			{
				verifyAndAddMove(move);
			}
		}
		if (((1UL << move.startingSquare) & ROW_7) != 0)
		{
			move.newSquare += SOUTH;
			if (((pieces[ALL] >> move.newSquare) & 1UL) == 0)
			{
				verifyAndAddMove(move);
			}
		}
		if (((1UL << move.startingSquare) & A_FILE) == 0)
		{
			pawnCapture(move, SOUTHWEST);
		}
		if (((1UL << move.startingSquare) & H_FILE) == 0)
		{
			pawnCapture(move, SOUTHEAST);
		}
	}
}

void	Board::generateKnightMoves()
{
	int square;
	int horizontalDir, verticalDir;
	Move move(0);

	getPieceIndexes(pieces[KNIGHT] & pieces[sideToMove]);
	for (int index = 0; pieceSQs[index] != -1 && index < 10; index++)
	{
		move.startingSquare = pieceSQs[index];
		for (int i = 0; i < 8; i++)
		{
			square = move.startingSquare + knightMoves[i];
			horizontalDir = knightMoves[i] % 8;
			verticalDir = knightMoves[i] / 8;
			if (move.startingSquare % 8 + horizontalDir < 0 || move.startingSquare % 8 + horizontalDir > 7)
			{
				continue;
			}
			if (move.startingSquare / 8 + verticalDir < 0 || move.startingSquare / 8 + verticalDir > 7)
			{
				continue;
			}
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
	for (int index = 0; pieceSQs[index] != -1 && index < 10; index++)
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
	for (int index = 0; pieceSQs[index] != -1 && index < 10; index++)
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
	for (int index = 0; pieceSQs[index] != -1 && index < 10; index++)
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
		if ((((pieces[KING] & pieces[WHITE]) >> E1) & 1UL) == 0)
		{
			return;
		}
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
		if ((((pieces[KING] & pieces[BLACK]) >> E8) & 1UL) == 0)
		{
			return;
		}
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

#include <chrono>

void	Board::generateMoves()
{
	moveList.clear();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	getPieceIndexes(pieces[KING] & pieces[sideToMove]);
	bool kingIsChecked = inCheck(pieceSQs[0]);

	position.set(pieces, indexBoard, sideToMove, enPassentSquare, castlingRights, halfMoveClock, fullMoveCount);
	std::cout << "Generated moves (none): " << moveList.size() << std::endl;
	generateRookMoves();
	std::cout << "Generated moves (rooks): " << moveList.size() << std::endl;
	generateBishopMoves();
	std::cout << "Generated moves: (bishops)" << moveList.size() << std::endl;
	generateKnightMoves();
	std::cout << "Generated moves: (knights)" << moveList.size() << std::endl;
	generateQueenMoves();
	std::cout << "Generated moves: (queens)" << moveList.size() << std::endl;
	generateKingMoves();
	std::cout << "Generated moves: (king)" << moveList.size() << std::endl;
	if (sideToMove == WHITE)
	{
		generateWhitePawnMoves();
	}
	else
	{
		generateBlackPawnMoves();
	}
	std::cout << "Generated moves: (pawns)" << moveList.size() << std::endl;
	if (kingIsChecked == false)
	{
		generateCastlingMoves();
	}
	std::cout << "Generated moves: (castling)" << moveList.size() << std::endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us" << std::endl;
}