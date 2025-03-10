#include "Board.hpp"

Board::Board()
{
	sideToMove = WHITE;
	enPassentSquare = -1;
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
	sideToMove = WHITE;
	enPassentSquare = -1;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	whitePieces = 0;
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
				pawns |= 1UL << i;
				whitePieces |= 1UL << i;
				break;

			case 'N':
				knights |= 1UL << i;
				whitePieces |= 1UL << i;
				break;

			case 'B':
				bishops |= 1UL << i;
				whitePieces |= 1UL << i;
				break;

			case 'R':
				rooks |= 1UL << i;
				whitePieces |= 1UL << i;
				break;

			case 'Q':
				queens |= 1UL << i;
				whitePieces |= 1UL << i;
				break;

			case 'K':
				kings |= 1UL << i;
				whitePieces |= 1UL << i;
				break;

			case 'p':
				pawns |= 1UL << i;
				break;

			case 'n':
				knights |= 1UL << i;
				break;

			case 'b':
				bishops |= 1UL << i;
				break;

			case 'r':
				rooks |= 1UL << i;
				break;

			case 'q':
				queens |= 1UL << i;
				break;

			case 'k':
				kings |= 1UL << i;
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
		board.pawns & board.whitePieces,
		board.knights & board.whitePieces,
		board.bishops & board.whitePieces,
		board.rooks & board.whitePieces,
		board.queens & board.whitePieces,
		board.kings & board.whitePieces,
		board.pawns & (~board.whitePieces),
		board.knights & (~board.whitePieces),
		board.bishops & (~board.whitePieces),
		board.rooks & (~board.whitePieces),
		board.queens & (~board.whitePieces),
		board.kings & (~board.whitePieces),
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
	out << "Castling rights: " << board.castlingRights << std::endl;
	out << "Half moves since last capture/pawn move: " << board.halfMoveClock << std::endl;
	out << "Move: " << board.fullMoveCount << std::endl;
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
