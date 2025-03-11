#include "Board.hpp"

/*	Loops until the edge of the board or the first piece is in the way.
	If enemy piece, square is included.	*/


static bool	isOnOuterFile(int square)
{
	return (((1UL << square) & A_FILE) != 0 || ((1UL << square) & H_FILE) != 0);
}

void	Board::searchDiagonalDirection(int direction, int square, u64& moveOptions)
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

void	Board::searchOrthogonalDirection(int direction, int square, u64& moveOptions)
{
	int test;

	while (square < 0 || square > 63)
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

	int kingIndex = 0;
	u64	king = kings & pieces[sideToMove];

	for (int i = 0; i < 64; i++)
	{
		if (((king >> i) & 1UL) == 1)
		{
			kingIndex = i;
			break;
		}
	}
	u64 moveOptions = 0;
	for (int i = 0; i < 8; i++)
	{
		int test = kingIndex % 8 + kingMoves[i] % 8;
		if (test < 0 || test > 7)
		{
			continue;
		}
		int square = kingIndex + kingMoves[i];
		if (square >= 0 && square < 64 && ((pieces[sideToMove] >> square) & 1UL) == 0)
		{
			moveOptions |= 1UL << square;
		}
	}
	moves.push_back({kingIndex, moveOptions});
}

void	Board::generateWhitePawnMoves()
{
	int index = 0;
	getPieceIndexes(knights & pieces[sideToMove]);
	while (pieceSQs[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		int square = pieceSQs[index] + NORTH;

		if (((pieces[ALL] >> square) & 1UL) == 0)
		{
			moveOptions |= 1UL << square;
		}
		square += NORTH;
		if ((pieceSQs[index] & ROW_1) != 0 && ((pieces[ALL] >> square) & 1UL) == 0)
		{
			moveOptions |= 1UL << square;
		}
		if ((pieceSQs[index] & A_FILE) != 0)
		{
			square = pieceSQs[index] + NORTHWEST;
			if (((pieces[BLACK] >> square) & 1UL) == 1)
			{
				moveOptions |= 1UL << square;
			}
		}
		if ((pieceSQs[index] & H_FILE) != 0)
		{
			square = pieceSQs[index] + NORTHEAST;
			if (((pieces[BLACK] >> square) & 1UL) == 1 || square == enPassentSquare)
			{
				moveOptions |= 1UL << square;
			}
		}
		moves.push_back({pieceSQs[index], moveOptions});
		index++;
	}
}

void	Board::generateBlackPawnMoves()
{
	int index = 0;
	getPieceIndexes(knights & pieces[sideToMove]);
	while (pieceSQs[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		int square = pieceSQs[index] + SOUTH;

		if (((pieces[WHITE] >> square) & 1UL) == 0 && ((pieces[BLACK] >> square) & 1UL) == 0)
		{
			moveOptions |= 1UL << square;
		}
		square += SOUTH;
		if (pieceSQs[index] <= blackPawnH7 && 
			(((pieces[WHITE] >> square) & 1UL) == 0 && \
			((pieces[BLACK] >> square) & 1UL) == 0))
		{
			moveOptions |= 1UL << square;
		}
		if (pieceSQs[index] % 8 != A_FILE)
		{
			square = pieceSQs[index] + SOUTHWEST;
			if (((pieces[WHITE] >> square) & 1UL) == 1)
			{
				moveOptions |= 1UL << square;
			}
		}
		if (pieceSQs[index] % 8 != H_FILE)
		{
			square = pieceSQs[index] + SOUTHEAST;
			if (((pieces[WHITE] >> square) & 1UL) == 1 || square == enPassentSquare)
			{
				moveOptions |= 1UL << square;
			}
		}
		moves.push_back({pieceSQs[index], moveOptions});
		index++;
	}
}

void	Board::generateKnightMoves()
{
	int index = 0;
	getPieceIndexes(knights & pieces[sideToMove]);
	while (pieceSQs[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		for (int i = 0; i < 8; i++)
		{
			int test = pieceSQs[index] % 8 + knightMoves[i] % 8;
			if (test < 0 || test > 7)
			{
				continue;
			}
			int square = pieceSQs[index] + knightMoves[i];
			if (square >= 0 && square < 64 && ((pieces[sideToMove] >> square) & 1UL) == 0)
			{
				moveOptions |= 1UL << square;
			}
		}
		moves.push_back({pieceSQs[index], moveOptions});
		index++;
	}
}

void	Board::generateQueenMoves()
{
	int index = 0;
	getPieceIndexes(queens & pieces[sideToMove]);
	while (pieceSQs[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		searchDiagonalDirection(NORTHWEST, pieceSQs[index], moveOptions);
		searchDiagonalDirection(NORTHEAST, pieceSQs[index], moveOptions);
		searchDiagonalDirection(SOUTHWEST, pieceSQs[index], moveOptions);
		searchDiagonalDirection(SOUTHEAST, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(WEST, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(EAST, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(NORTH, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(SOUTH, pieceSQs[index], moveOptions);
		moves.push_back({pieceSQs[index], moveOptions});
		index++;
	}
}

void	Board::generateBishopMoves()
{
	int index = 0;
	getPieceIndexes(bishops & pieces[sideToMove]);
	while (pieceSQs[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		searchDiagonalDirection(NORTHWEST, pieceSQs[index], moveOptions);
		searchDiagonalDirection(NORTHEAST, pieceSQs[index], moveOptions);
		searchDiagonalDirection(SOUTHWEST, pieceSQs[index], moveOptions);
		searchDiagonalDirection(SOUTHEAST, pieceSQs[index], moveOptions);
		moves.push_back({pieceSQs[index], moveOptions});
		index++;
	}
}

void	Board::generateRookMoves()
{
	int index = 0;
	getPieceIndexes(rooks & pieces[sideToMove]);
	while (pieceSQs[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		searchOrthogonalDirection(WEST, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(EAST, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(NORTH, pieceSQs[index], moveOptions);
		searchOrthogonalDirection(SOUTH, pieceSQs[index], moveOptions);
		moves.push_back({pieceSQs[index], moveOptions});
		index++;
	}
}

void	Board::generateCastlingMoves()
{
	u64	allPieces = pieces[WHITE] | pieces[BLACK];

	if (sideToMove == WHITE)
	{
		if ((castlingRights & whiteKingSide) != 0)
		{
			if (inCheck(F1) == false && inCheck(G1) == false)
			{
				if ((allPieces >> F1) & 1UL == 0 && (allPieces >> G1) & 1UL == 0)
				{
					castlingRights ^= whiteKingSide;
				}
			}
		}
		if ((castlingRights & whiteQueenSide) != 0)
		{
			if (inCheck(D1) == false && inCheck(C1) == false)
			{
				if ((allPieces >> D1) & 1UL == 0 && (allPieces >> C1) & 1UL == 0)
				{
					castlingRights ^= whiteQueenSide;
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
				if ((allPieces >> F8) & 1UL == 0 && (allPieces >> G8) & 1UL == 0)
				{
					castlingRights ^= blackKingSide;
				}
			}
		}
		if ((castlingRights & blackQueenSide) != 0)
		{
			if (inCheck(D8) == false && inCheck(C8) == false)
			{
				if ((allPieces >> D8) & 1UL == 0 && (allPieces >> C8) & 1UL == 0)
				{
					castlingRights ^= blackQueenSide;
				}
			}
		}
	}
}


void	Board::generateMoves()
{
	// moves.clear();
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
	generateCastlingMoves();
	for (const std::pair<int, u64>& move : moves)
	{
		std::cout << "Piece: " << getPiece(move.first) << std::endl;
		printBitboard(move.second);
	}
}