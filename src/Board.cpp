#include "Board.hpp"

Board::Board() : blackCanCastle(true), whiteCanCastle(true), sideToMove(WHITE), enPassentSquare(0)
{
	whitePawns = 0;
	blackPawns = 0;
	whiteKnights = 0;
	blackKnights = 0;
	whiteBishops = 0;
	blackBishops = 0;
	whiteRooks = 0;
	blackRooks = 0;
	whiteQueens = 0;
	blackQueens = 0;
	whiteKing = 0;
	blackKing = 0;
	parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

Board::Board(const std::string& fen)
{
	
}

Board::Board(const Board& other)
{
	if (&other != this)
	{
		*this = other;
	}
}

Board&	Board::operator=(const Board& other)
{
	if (this != &other)
	{
		this->blackCanCastle = other.blackCanCastle;
		this->blackCanCastle = other.blackCanCastle;
		this->sideToMove = other.sideToMove;
		this->whitePawns = other.whitePawns;
		this->blackPawns = other.blackPawns;
		this->whiteKnights = other.whiteKnights;
		this->blackKnights = other.blackKnights;
		this->whiteBishops = other.whiteBishops;
		this->blackBishops = other.blackBishops;
		this->whiteRooks = other.whiteRooks;
		this->blackRooks = other.blackRooks;
		this->whiteQueens = other.whiteQueens;
		this->blackQueens = other.blackQueens;
		this->whiteKing = other.whiteKing;
		this->blackKing = other.blackKing;
	}
	return (*this);
}

void	Board::parseFen(const char* fen)
{
	int i = 0;

	while (i < 64 && *fen != '\0')
	{
		switch (*fen)
		{
			case 'P':
				whitePawns |= 1 << i;

			case 'N':
				whiteKnights |= 1 << i;

			case 'B':
				whiteBishops |= 1 << i;

			case 'R':
				whiteRooks |= 1 << i;

			case 'Q':
				whiteQueens |= 1 << i;

			case 'K':
				whiteKing = 1 << i;

			case 'p':
				blackPawns |= 1 << i;

			case 'n':
				blackKnights |= 1 << i;

			case 'b':
				blackBishops |= 1 << i;

			case 'r':
				blackRooks |= 1 << i;

			case 'q':
				blackQueens |= 1 << i;

			case 'k':
				blackKing = 1 << i;

			case '/':
				i--;

			default:
				i += *fen - '0';
		}
		i++;
		fen++;
	}
}
