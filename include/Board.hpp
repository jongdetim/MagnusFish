#pragma once

#include "MagnusFish.hpp"
#include <map>

enum colors
{
	WHITE,
	BLACK
};

enum castlingRights
{
	whiteKingSide = 1,
	whiteQueenSide = 1 << 1,
	blackKingSide = 1 << 2,
	blackQueenSide = 1 << 3,
};

enum pieces
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

class Board
{
	public:

	Board();
	Board(const std::string& fen);
	~Board() = default;
	Board(const Board& other);

	Board&	operator=(const Board& other);

	/*	Miscellaneous states	*/

	bool	sideToMove;
	i16		enPassentSquare;
	i16		castlingRights;
	i16		halfMoveClock;
	i16		fullMoveCount;

	/*	Bitboards of pieces	*/

	u64		pawns;
	u64		knights;
	u64		bishops;
	u64		rooks;
	u64		queens;
	u64		kings;
	u64		whitePieces;

	private:

	void	parseFen(const char* fen);
};

std::ostream&	operator<<(std::ostream& out, const Board& board);
