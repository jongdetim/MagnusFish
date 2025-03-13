#include "Position.hpp"

void	Position::set(

	std::array<u64, 9> pieces,
	std::array<int, 64> indexBoard,
	bool sideToMove,
	int enPassentSquare,
	int castlingRights,
	int halfMoveClock,
	int fullMoveCount)
{
	this->pieces = pieces;
	this->indexBoard = indexBoard;
	this->sideToMove = sideToMove;
	this->enPassentSquare = enPassentSquare;
	this->castlingRights = castlingRights;
	this->halfMoveClock = halfMoveClock;
	this->fullMoveCount = fullMoveCount;
}

Position::Position(const Position& other)
{
	std::memcpy(this, &other, sizeof(Position));
}

Position&	Position::operator=(const Position& other)
{
	std::memcpy(this, &other, sizeof(Position));
	return (*this);
}