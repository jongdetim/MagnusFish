#include "Board.hpp"

Board::Board()
{
	sideToMove = WHITE;
	enPassentSquare = 0;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	whitePieces = 0;
	parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
		this->sideToMove = other.sideToMove;
		this->enPassentSquare = other.enPassentSquare;
		this->castlingRights = other.castlingRights;
		this->sideToMove = other.sideToMove;
		this->pawns = other.pawns;
		this->knights = other.knights;
		this->bishops = other.bishops;
		this->rooks = other.rooks;
		this->queens = other.queens;
		this->kings = other.kings;
		this->whitePieces = other.whitePieces;
	}
	return (*this);
}

void	Board::parseFen(const char* fen)
{
	int i = 0;

	while (i < 64 && *fen != ' ')
	{
		switch (*fen)
		{
			case 'P':
				pawns |= 1 << i;

			case 'N':
				knights |= 1 << i;

			case 'B':
				bishops |= 1 << i;

			case 'R':
				rooks |= 1 << i;

			case 'Q':
				queens |= 1 << i;

			case 'K':
				kings = 1 << i;

			case 'p':
				pawns |= 1 << i;

			case 'n':
				knights |= 1 << i;

			case 'b':
				bishops |= 1 << i;

			case 'r':
				rooks |= 1 << i;

			case 'q':
				queens |= 1 << i;

			case 'k':
				kings = 1 << i;

			case '/':
				i--;

			default:
				i += *fen - '0';
		}
		i++;
		fen++;
	}
	fen++;
	if (*fen == 'w')
	{
		sideToMove = WHITE;
	}
	else
	{
		sideToMove = BLACK;
	}
	while (*fen != ' ')
	{
		switch (*fen)
		{
			case 'K':
				castlingRights |= whiteKingSide;

			case 'Q':
				castlingRights |= whiteQueenSide;

			case 'k':
				castlingRights |= blackKingSide;

			case 'q':
				castlingRights |= blackQueenSide;
			
			default:
				;
		}
		fen++;
	}
	fen++;
	if (*fen != '-')
	{
		enPassentSquare = (8 - fen[0] - 'a') * 8 + fen[1] - '0';
		fen++;
	}
	fen++;
	halfMoveClock = atoi(fen);
	while (*fen != ' ')
	{
		fen++;
	}
	fullMoveCount = atoi(fen);
}
