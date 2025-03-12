#include "Board.hpp"

Board::Board()
{
	sideToMove = WHITE;
	enPassentSquare = -1;
	castlingRights = 0;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	pieces[WHITE] = 0;
	pieces[BLACK] = 0;
	pieces[ALL] = 0;
	kingIsChecked = false;
	halfMoveClock = 0;
	fullMoveCount = 1;
}

Board::Board(const std::string& fen)
{
	sideToMove = WHITE;
	enPassentSquare = -1;
	castlingRights = 0;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	pieces[WHITE] = 0;
	pieces[BLACK] = 0;
	pieces[ALL] = 0;
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
		this->pieces[ALL] = other.pieces[ALL];
		this->kingIsChecked = other.kingIsChecked;
		this->halfMoveClock = other.halfMoveClock;
		this->fullMoveCount = other.fullMoveCount;
	}
	return (*this);
}

void	Board::reset()
{
	sideToMove = WHITE;
	enPassentSquare = -1;
	castlingRights = 0;
	pawns = 0;
	knights = 0;
	bishops = 0;
	rooks = 0;
	queens = 0;
	kings = 0;
	pieces[WHITE] = 0;
	pieces[BLACK] = 0;
	pieces[ALL] = 0;
	kingIsChecked = false;
	halfMoveClock = 0;
	fullMoveCount = 0;
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
	pieces[ALL] = pieces[WHITE] | pieces[BLACK];
	fen++;
	std::cout << fen << std::endl;
	sideToMove = WHITE;
	if (*fen == 'b')
	{
		sideToMove = BLACK;
	}
	fen += 2;
	if (*fen == '-')
	{
		fen += 2;
	}
	else
	{
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
				
				case '-':
					break;

				default:
					;
			}
			fen++;
		}
		fen++;
	}
	if (*fen != '-')
	{
		std::cout << "file: " << (int)(fen[0] - 'a') << " rank: " << (int)(8 - (fen[1] - '0')) << std::endl;
		enPassentSquare =  8 * (8 - (fen[1] - '0')) + (fen[0] - 'a');
		fen++;
	}
	fen += 2;
	halfMoveClock = atoi(fen);
	while (*fen != ' ')
	{
		fen++;
	}
	fen++;
	fullMoveCount = atoi(fen);
}

std::ostream& operator<<(std::ostream& out, const Board& board)
{
    const std::string RESET = "\033[0m";
	const std::string GREEN = "\033[0;32m";

    std::vector<std::string> visualBoard(64, "  ");
    std::array<u64, 12> pieces =
    {
        board.pawns & board.pieces[WHITE],
        board.knights & board.pieces[WHITE],
        board.bishops & board.pieces[WHITE],
        board.rooks & board.pieces[WHITE],
        board.queens & board.pieces[WHITE],
        board.kings & board.pieces[WHITE],
        board.pawns & (board.pieces[BLACK]),
        board.knights & (board.pieces[BLACK]),
        board.bishops & (board.pieces[BLACK]),
        board.rooks & (board.pieces[BLACK]),
        board.queens & (board.pieces[BLACK]),
        board.kings & (board.pieces[BLACK]),
    };
    std::map<int, std::string> pieceSymbols =
    {
		{0, "♟ "}, {1, "♞ "}, {2, "♝ "},
		{3, "♜ "}, {4, "♛ "}, {5, "♚ "},
		{6, "♙ "}, {7, "♘ "}, {8, "♗ "},
		{9, "♖ "}, {10, "♕ "}, {11, "♔ "}
    };

    for (int pos = 0; pos < 12; pos++)
    {
        for (size_t square = 0; square < 64; square++)
        {
            if (((pieces[pos] >> (63 - square)) & 1UL) == 1)
            {
                visualBoard[square] = pieceSymbols.at(pos);
            }
        }
    }

    out << "sideToMove: ";
    if (board.sideToMove == WHITE)
	{
		out << "white";
	}
    else
	{
		out << "black";
	}
    out << std::endl;
    out << "Castling rights: ";
    for (int i = 0; i < 4; i++)
    {
        if (((board.castlingRights >> i) & 1UL) == 1)
        {
            out << "Y";
        }
        else
        {
            out << "N";
        }
    }
    out << std::endl;
	if (board.enPassentSquare != -1)
	{
    	out << "En passent square: " << board.enPassentSquare << std::endl;
	}
	if (board.halfMoveClock != 0)
	{
	    out << "Half moves since last capture/pawn move: " << (int)board.halfMoveClock << std::endl;
	}
	if (board.fullMoveCount != 1)
	{
	    out << "Move: " << (int)board.fullMoveCount << std::endl;
	}
    
	out << "\n  +----------------+\n";
	for (int rank = 7; rank >= 0; rank--)
	{
		out << (rank + 1) << " |";
		for (int file = 7; file >= 0; file--)
		{
			int square = rank * 8 + file;
			out << GREEN;
			out << visualBoard[square] << RESET;
		}
		out << "|\n";
	}
	out << "  +----------------+\n";
	out << "   a b c d e f g h\n";
    return (out);
}