#include "Board.hpp"

Board::Board()
{
	sideToMove = WHITE;
	enPassentSquare = -1;
	castlingRights = whiteKingSide | whiteQueenSide | blackKingSide | blackQueenSide;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	pieces[WHITE] = 0;
	pieces[BLACK] = 0;
	kingIsChecked = false;
	halfMoveClock = 0;
	fullMoveCount = 1;
	parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

Board::Board(const std::string& fen)
{
	sideToMove = WHITE;
	enPassentSquare = -1;
	castlingRights = whiteKingSide | whiteQueenSide | blackKingSide | blackQueenSide;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	pieces[WHITE] = 0;
	pieces[BLACK] = 0;
	kingIsChecked = false;
	halfMoveClock = 0;
	fullMoveCount = 1;
	parseFen(fen.c_str());
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
		this->pieces[WHITE] = other.pieces[WHITE];
		this->pieces[BLACK] = other.pieces[BLACK];
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
				pawns |= 1UL << i;
				pieces[WHITE] |= 1UL << i;
				break;

			case 'N':
				knights |= 1UL << i;
				pieces[WHITE] |= 1UL << i;
				break;

			case 'B':
				bishops |= 1UL << i;
				pieces[WHITE] |= 1UL << i;
				break;

			case 'R':
				rooks |= 1UL << i;
				pieces[WHITE] |= 1UL << i;
				break;

			case 'Q':
				queens |= 1UL << i;
				pieces[WHITE] |= 1UL << i;
				break;

			case 'K':
				kings |= 1UL << i;
				pieces[WHITE] |= 1UL << i;
				break;

			case 'p':
				pawns |= 1UL << i;
				pieces[BLACK] |= 1UL << i;
				break;

			case 'n':
				knights |= 1UL << i;
				pieces[BLACK] |= 1UL << i;
				break;

			case 'b':
				bishops |= 1UL << i;
				pieces[BLACK] |= 1UL << i;
				break;

			case 'r':
				rooks |= 1UL << i;
				pieces[BLACK] |= 1UL << i;
				break;

			case 'q':
				queens |= 1UL << i;
				pieces[BLACK] |= 1UL << i;
				break;

			case 'k':
				kings |= 1UL << i;
				pieces[BLACK] |= 1UL << i;
				break;

			case '/':
				i--;
				break;

			default:
				i += *fen - '0' - 1;
		}
		i++;
		fen++;
	}
	fen++;
	sideToMove = WHITE;
	if (*fen == 'b')
	{
		sideToMove = BLACK;
	}
	fen += 2;
	while (*fen != ' ')
	{
		switch (*fen)
		{
			case 'K':
				castlingRights |= whiteKingSide;
				break;

			case 'Q':
				castlingRights |= whiteQueenSide;
				break;

			case 'k':
				castlingRights |= blackKingSide;
				break;

			case 'q':
				castlingRights |= blackQueenSide;
				break;
			
			default:
				;
		}
		fen++;
	}
	fen++;
	pieces[ALL] = pieces[WHITE] | pieces[BLACK];
	std::cout << fen << std::endl;
	if (*fen != '-')
	{
		enPassentSquare = (8 - fen[0] - 'a') * 8 + fen[1] - '0';
		fen++;
	}
	fen += 2;
	std::cout << fen << std::endl;
	halfMoveClock = atoi(fen);
	while (*fen != ' ')
	{
		fen++;
	}
	fen++;
	std::cout << fen << std::endl;
	fullMoveCount = atoi(fen);
}

std::ostream&	operator<<(std::ostream& out, const Board& board)
{
	std::vector<char>	visualBoard(64, '.');
	std::array<u64, 12>		pieces =
	{
		board.pawns & board.pieces[WHITE],
		board.knights & board.pieces[WHITE],
		board.bishops & board.pieces[WHITE],
		board.rooks & board.pieces[WHITE],
		board.queens & board.pieces[WHITE],
		board.kings & board.pieces[WHITE],
		board.pawns & (~board.pieces[WHITE]),
		board.knights & (~board.pieces[WHITE]),
		board.bishops & (~board.pieces[WHITE]),
		board.rooks & (~board.pieces[WHITE]),
		board.queens & (~board.pieces[WHITE]),
		board.kings & (~board.pieces[WHITE]),
	};
	std::map<int, char>	intToChar =
	{
		{0, 'P'}, {1, 'N'},	{2, 'B'},
		{3, 'R'}, {4, 'Q'},	{5, 'K'},
		{6, 'p'}, {7, 'n'}, {8, 'b'},
		{9, 'r'}, {10, 'q'}, {11, 'k'}
	};

	// for (int i = 0; i < 12; i++)
	// {
	// 	std::cout << "Bitboard of: " << intToChar.at(i) << std::endl;
	// 	printBitboard(pieces[i]);
	// }
	for (int pos = 0; pos < 12; pos++)
	{
		for (size_t square = 0; square < 64; square++)
		{
			if (((pieces[pos] >> square) & 1UL) == 1)
			{
				visualBoard[square] = intToChar.at(pos);
			}
		}
	}
	out << "sideToMove: ";
	if (board.sideToMove == WHITE)
		out << "white";
	else
		out << "black";
	out << std::endl;
	out << "En passent square: " << board.enPassentSquare << std::endl;
	out << "Castling rights: ";
	for (int i = 0; i < 4; i++)
	{
		if (((board.castlingRights >> i) & 1) == 1)
		{
			out << "Y";
		}
		else
		{
			out << "N";
		}
	}
	out << std::endl;
	out << "Half moves since last capture/pawn move: " << (int)board.halfMoveClock << std::endl;
	out << "Move: " << (int)board.fullMoveCount << std::endl;
	out << "\nBoard:";
	for (int x = 0; x < 64; x++)
	{
		if (x % 8 == 0)
		{
			out << std::endl;
		}
		out << visualBoard[x];
	}
	return (out);
}
