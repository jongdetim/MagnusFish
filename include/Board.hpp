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

enum directions
{
	NORTHWEST = -9,
	NORTH = -8,
	NORTHEAST = -7,
	WEST = -1,
	EAST = 1,
	SOUTHWEST = 7,
	SOUTH = 8,
	SOUTHEAST = 9
};

enum startingPawns
{
	whitePawnA2 = 6 * 8,
	whitePawnH2 = 6 * 8 + 7,
	blackPawnA7 = 8,
	blackPawnH7 = 8 + 7
};

enum files
{
	A_FILE = 0,
	H_FILE = 7
};

class Board
{
	public:

	Board();
	Board(const std::string& fen);
	~Board() = default;
	Board(const Board& other);

	Board&	operator=(const Board& other);

	void	generateMoves();
	void	getPieceIndexes(u64 bitboard);
	void	searchDirection(int direction, int square, u64& moves);

	void	generateBishopMoves();
	void	generateBlackPawnMoves();
	void	generateKingMoves();
	void	generateKnightMoves();
	void	generateQueenMoves();
	void	generateRookMoves();
	void	generateWhitePawnMoves();

	/*	Miscellaneous states	*/

	bool	sideToMove;
	int		enPassentSquare;
	int		castlingRights;
	int		halfMoveClock;
	int		fullMoveCount;

	/*	Bitboards of pieces	*/

	u64		pawns;
	u64		knights;
	u64		bishops;
	u64		rooks;
	u64		queens;
	u64		kings;
	u64		pieces[2];

	std::vector<std::pair<int, u64>>	moves;

	private:

	void	parseFen(const char* fen);
	std::array<int, 10> pieceLocations;
};

std::ostream&	operator<<(std::ostream& out, const Board& board);
