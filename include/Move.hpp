#include "MagnusFish.hpp"

struct Move
{
	int		startingSquare;
	int		newSquare;
	bool	isCapture;
	bool	isEnPassent;
	bool	isPromotion;
	bool	isCastle;
	bool	isCheck;
};