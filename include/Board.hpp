#pragma once

#include "MagnusFish.hpp"

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

class Board
{
	public:

	Board();
	Board(const std::string& fen);
	~Board() = default;
	Board(const Board& other);

	Board&	operator=(const Board& other);

	/*	Miscellaneous states*/

	bool	sideToMove;
	int8_t	enPassentSquare;
	int8_t	castlingRights;
	int16_t	halfMoveClock;
	int16_t	fullMoveCount;

	/*	Bitboards of pieces	*/

	int64_t	pawns;
	int64_t	knights;
	int64_t	bishops;
	int64_t	rooks;
	int64_t	queens;
	int64_t	kings;
	int64_t	whitePieces;

	private:

	void	parseFen(const char* fen);
};

std::ostream&	operator<<(std::ostream& out, const Board& board)
{
	std::string	visualBoard;

	visualBoard.resize(64);

}
