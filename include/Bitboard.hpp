#pragma once

#include "MagnusFish.hpp"

class Bitboard
{
	public:

	Bitboard();
	u64	board;

	private:
};

std::ostream&	operator<<(std::ostream& out, const Bitboard& board);