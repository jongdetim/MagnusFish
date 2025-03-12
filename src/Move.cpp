#include "Move.hpp"
#include "Board.hpp"

std::ostream&	operator<<(std::ostream& out, const Move& move)
{
	out << "Starting square: " << move.startingSquare << std::endl;
	out << "New square: " << move.newSquare << std::endl;
	out << "Captured piece: " << move.capturedPiece << std::endl;
	out << "Promotes to: " << move.promotesTo << std::endl;
	out << "Castle: " << move.castle << std::endl;
	return (out);
}

void	Move::reset()
{
	newSquare = 0;
	capturedPiece = 0;
	promotesTo = 0;
	castle = 0;
}