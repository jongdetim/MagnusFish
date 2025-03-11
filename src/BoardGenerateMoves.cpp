#include "Board.hpp"

/*	Loops until the edge of the board or the first piece is in the way.
	If enemy piece, square is included.	*/

void	Board::searchDirection(int direction, int square, u64& moveOptions)
{
	while (true)
	{
		// std::cout << "dir % 8: " << (direction % 8) << std::endl;
		int test = square % 8 + direction % 8;
		if (test < 0 || test > 7)
		{
			break;
		}
		square += direction;
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
	while (pieceLocations[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		int square = pieceLocations[index] + NORTH;

		if (((pieces[WHITE] >> square) & 1UL) == 0 && ((pieces[BLACK] >> square) & 1UL) == 0)
		{
			moveOptions |= 1UL << square;
		}
		square += NORTH;
		if (pieceLocations[index] >= whitePawnA2 && 
			((pieces[WHITE] >> square) & 1UL) == 0 && \
			((pieces[BLACK] >> square & 1UL) == 0))
		{
			moveOptions |= 1UL << square;
		}
		if (pieceLocations[index] % 8 != A_FILE)
		{
			square = pieceLocations[index] + NORTHWEST;
			if (((pieces[BLACK] >> square) & 1UL) == 1)
			{
				moveOptions |= 1UL << square;
			}
		}
		if (pieceLocations[index] % 8 != H_FILE)
		{
			square = pieceLocations[index] + NORTHEAST;
			if (((pieces[BLACK] >> square) & 1UL) == 1 || square == enPassentSquare)
			{
				moveOptions |= 1UL << square;
			}
		}
		moves.push_back({pieceLocations[index], moveOptions});
		index++;
	}
}

void	Board::generateBlackPawnMoves()
{
	int index = 0;
	getPieceIndexes(knights & pieces[sideToMove]);
	while (pieceLocations[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		int square = pieceLocations[index] + SOUTH;

		if (((pieces[WHITE] >> square) & 1UL) == 0 && ((pieces[BLACK] >> square) & 1UL) == 0)
		{
			moveOptions |= 1UL << square;
		}
		square += SOUTH;
		if (pieceLocations[index] <= blackPawnH7 && 
			(((pieces[WHITE] >> square) & 1UL) == 0 && \
			((pieces[BLACK] >> square) & 1UL) == 0))
		{
			moveOptions |= 1UL << square;
		}
		if (pieceLocations[index] % 8 != A_FILE)
		{
			square = pieceLocations[index] + SOUTHWEST;
			if (((pieces[WHITE] >> square) & 1UL) == 1)
			{
				moveOptions |= 1UL << square;
			}
		}
		if (pieceLocations[index] % 8 != H_FILE)
		{
			square = pieceLocations[index] + SOUTHEAST;
			if (((pieces[WHITE] >> square) & 1UL) == 1 || square == enPassentSquare)
			{
				moveOptions |= 1UL << square;
			}
		}
		moves.push_back({pieceLocations[index], moveOptions});
		index++;
	}
}

void	Board::generateKnightMoves()
{
	constexpr std::array<int, 8>	knightMoves =
	{
		2 * NORTH + WEST,
		2 * NORTH + EAST, 
		2 * EAST + NORTH,
		2 * EAST + SOUTH,
		2 * SOUTH + EAST,
		2 * SOUTH + WEST,
		2 * WEST + SOUTH,
		2 * WEST + NORTH
	};

	int index = 0;
	getPieceIndexes(knights & pieces[sideToMove]);
	while (pieceLocations[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		for (int i = 0; i < 8; i++)
		{
			int test = pieceLocations[index] % 8 + knightMoves[i] % 8;
			if (test < 0 || test > 7)
			{
				continue;
			}
			int square = pieceLocations[index] + knightMoves[i];
			if (square >= 0 && square < 64 && ((pieces[sideToMove] >> square) & 1UL) == 0)
			{
				moveOptions |= 1UL << square;
			}
		}
		moves.push_back({pieceLocations[index], moveOptions});
		index++;
	}
}

void	Board::generateQueenMoves()
{
	int index = 0;
	getPieceIndexes(queens & pieces[sideToMove]);
	while (pieceLocations[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		searchDirection(NORTHWEST, pieceLocations[index], moveOptions);
		searchDirection(NORTHEAST, pieceLocations[index], moveOptions);
		searchDirection(SOUTHWEST, pieceLocations[index], moveOptions);
		searchDirection(SOUTHEAST, pieceLocations[index], moveOptions);
		searchDirection(WEST, pieceLocations[index], moveOptions);
		searchDirection(EAST, pieceLocations[index], moveOptions);
		searchDirection(NORTH, pieceLocations[index], moveOptions);
		searchDirection(SOUTH, pieceLocations[index], moveOptions);
		moves.push_back({pieceLocations[index], moveOptions});
		index++;
	}
}

void	Board::generateBishopMoves()
{
	int index = 0;
	getPieceIndexes(bishops & pieces[sideToMove]);
	while (pieceLocations[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		searchDirection(NORTHWEST, pieceLocations[index], moveOptions);
		searchDirection(NORTHEAST, pieceLocations[index], moveOptions);
		searchDirection(SOUTHWEST, pieceLocations[index], moveOptions);
		searchDirection(SOUTHEAST, pieceLocations[index], moveOptions);
		moves.push_back({pieceLocations[index], moveOptions});
		index++;
	}
}

void	Board::generateRookMoves()
{
	int index = 0;
	getPieceIndexes(rooks & pieces[sideToMove]);
	while (pieceLocations[index] != 0 && index < 10)
	{
		u64 moveOptions = 0;
		searchDirection(WEST, pieceLocations[index], moveOptions);
		searchDirection(EAST, pieceLocations[index], moveOptions);
		searchDirection(NORTH, pieceLocations[index], moveOptions);
		searchDirection(SOUTH, pieceLocations[index], moveOptions);
		moves.push_back({pieceLocations[index], moveOptions});
		index++;
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
	for (const std::pair<int, u64>& move : moves)
	{
		std::cout << "Piece: " << move.first << std::endl;
		printBitboard(move.second);
	}
}