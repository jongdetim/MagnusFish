#pragma once

#include "MagnusFish.hpp"
#include "Move.hpp"
#include <map>

enum colors
{
	WHITE,
	BLACK,
	ALL
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

enum squares
{
	A8 = 0, B8 = 1,	C8 = 2, D8 = 3, E8 = 4,	F8 = 5,	G8 = 6,	H8 = 7,
	A7 = 8, B7 = 9,	C7 = 10, D7 = 11, E7 = 12, F7 = 13, G7 = 14, H7 = 15,
	A6 = 16, B6 = 17, C6 = 18, D6 = 19, E6 = 20, F6 = 21, G6 = 22, H6 = 23,
	A5 = 24, B5 = 25, C5 = 26, D5 = 27, E5 = 28, F5 = 29, G5 = 30, H5 = 31,
	A4 = 32, B4 = 33, C4 = 34, D4 = 35, E4 = 36, F4 = 37, G4 = 38, H4 = 39,
	A3 = 40, B3 = 41, C3 = 42, D3 = 43, E3 = 44, F3 = 45, G3 = 46, H3 = 47,
	A2 = 48, B2 = 49, C2 = 50, D2 = 51, E2 = 52, F2 = 53, G2 = 54, H2 = 55,
	A1 = 56, B1 = 57, C1 = 58, D1 = 59, E1 = 60, F1 = 61, G1 = 62, H1 = 63
};

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
	char	getPiece(int square);
	void	searchDirection(int direction, int square, u64& moves);

	/*	Move generators	*/

	void	generateBishopMoves();
	void	generateBlackPawnMoves();
	void	generateCastlingMoves();
	void	generateKingMoves();
	void	generateKnightMoves();
	void	generateQueenMoves();
	void	generateRookMoves();
	void	generateWhitePawnMoves();

	/*	Checking for checks	*/
	
	bool	inCheck();
	bool	seesBishop(int king, int bishop);
	bool	seesKnight(int king, int knight);
	void	makeMove(Move move);

	/*	Miscellaneous states	*/

	bool	sideToMove;
	bool	kingIsChecked;
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
	u64		pieces[3];

	std::vector<std::pair<int, u64>>	moves;

	void	parseFen(const char* fen);
	std::array<int, 10> pieceLocations;
};

std::ostream&	operator<<(std::ostream& out, const Board& board);
