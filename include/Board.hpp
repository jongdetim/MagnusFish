#pragma once

#include "Move.hpp"
#include "Position.hpp"
#include <cstring>
#include <map>
#include <array>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using u8 = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;

enum castlingRights
{
	whiteKingSide = 1,
	whiteQueenSide = 1UL << 1,
	blackKingSide = 1UL << 2,
	blackQueenSide = 1UL << 3,
};

enum pieces
{
	WHITE,
	BLACK,
	ALL,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NONE,
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

enum files
{
	A_FILE = 0x0101010101010101,
	B_FILE = 0x0202020202020202,
	C_FILE = 0x0404040404040404,
	D_FILE = 0x0808080808080808,
	E_FILE = 0x1010101010101010,
	F_FILE = 0x2020202020202020,
	G_FILE = 0x4040404040404040,
	H_FILE = 0x8080808080808080
};

enum rows
{
	ROW_1 = 0xFF00000000000000,
	ROW_2 = 0x00FF000000000000,
	ROW_3 = 0x0000FF0000000000,
	ROW_4 = 0x000000FF00000000,
	ROW_5 = 0x00000000FF000000,
	ROW_6 = 0x0000000000FF0000,
	ROW_7 = 0x000000000000FF00,
	ROW_8 = 0x00000000000000FF
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

struct Position;

class Board
{
	public:

	Board();
	Board(const std::string& fen);
	~Board() = default;
	Board(const Board& other);

	Board&	operator=(const Board& other);

	void	reset();
	void	parseFen(const char* fen);

	/*	Helpers for searching	*/

	void	getPieceIndexes(u64 bitboard);
	bool	compareSquares(int square);
	void	searchOrthogonalDirection(int direction, int square);
	void	searchDiagonalDirection(int direction, int square);

	/*	Move generators	*/

	void	generateMoves();
	void	generateBishopMoves();
	void	generateBlackPawnMoves();
	void	generateCastlingMoves();
	void	generateKingMoves();
	void	generateKnightMoves();
	void	generateQueenMoves();
	void	generateRookMoves();
	void	generateWhitePawnMoves();

	void	pawnCapture(Move& move, int direction);
	void	promote(Move& move);

	/*	Checking for checks	*/
	
	bool	inCheck(int kingIndex);
	bool	inKnightCheck(int king);
	bool	orthogonallyInCheck(int king);
	bool	diagonallyInCheck(int king);
	bool	inPawnCheck(int kingIndex);
	void	searchDiagonally(int direction, int square, u64& moveOptions);
	void	searchOrthogonally(int direction, int square, u64& moveOptions);

	/*	Executing moves	*/

	void	verifyAndAddMove(const Move& move);
	void	makeMove(const Move& move);
	void	undoMove();
	void	castle(int castleDirection);
	void	uncastle(int castleDirection);

	/*	Miscellaneous states	*/

	bool	sideToMove;
	bool	kingIsChecked;
	int		enPassentSquare;
	int		castlingRights;
	int		halfMoveClock;
	int		fullMoveCount;

	/*	Bitboards of pieces	*/

	std::array<u64, 9>	pieces;
	std::vector<Move>	moveList;
	std::array<int, 10> pieceSQs;
	std::array<int, 64> indexBoard;

	Position			position;
};

std::ostream&	operator<<(std::ostream& out, const Board& board);
void	run();
void	printBitboard(u64 bitboard);
