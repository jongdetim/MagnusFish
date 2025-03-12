#pragma once

#include "Board.hpp"

struct Move
{
	int		startingSquare;
	int		newSquare;
	int		capturedPiece;
	bool	isEnPassent;
	bool	isPromotion;
	int		castle;
};