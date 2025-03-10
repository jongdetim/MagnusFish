#pragma once

#include "MagnusFish.hpp"

enum colors
{
	WHITE,
	BLACK
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

	bool	blackCanCastle;
	bool	whiteCanCastle;
	colors	sideToMove;
	int8_t	enPassentSquare;

	/*	Bitboards of pieces	*/

	int64_t	whitePawns;
	int64_t	blackPawns;
	int64_t	whiteKnights;
	int64_t	blackKnights;
	int64_t	whiteBishops;
	int64_t	blackBishops;
	int64_t	whiteRooks;
	int64_t	blackRooks;
	int64_t	whiteQueens;
	int64_t	blackQueens;
	int64_t	whiteKing;
	int64_t	blackKing;

	private:

	void	parseFen(const char* fen);
};

std::ostream&	operator<<(std::ostream& out, const Board& board)
{
	std::string	visualBoard;

	visualBoard.resize(64);
	
}
