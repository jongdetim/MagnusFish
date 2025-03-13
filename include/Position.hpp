#pragma once

#include <array>
#include <cstring>
#include <cinttypes>

using u64 = uint64_t;

struct Position
{
	Position() = default;
	Position(const Position& other);
	~Position() = default;
	Position&	operator=(const Position& other);

	void	set(std::array<u64, 9> pieces, std::array<int, 64> indexBoard, bool sideToMove, \
				int enPassentSquare, int castlingRights, int halfMoveClock, int fullMoveCount);

	std::array<u64, 9>	pieces;
	std::array<int, 64>	indexBoard;
	bool	sideToMove;
	int		enPassentSquare;
	int		castlingRights;
	int		halfMoveClock;
	int		fullMoveCount;
};