#pragma once

#include <iostream>

struct Move
{
	Move(int startSq) : startingSquare(startSq), newSquare(0), capturedPiece(0), promotesTo(0), castle(0) {}

	int		startingSquare;
	int		newSquare;
	int		capturedPiece;
	int		promotesTo;
	int		castle;
	void	reset();
};

std::ostream&	operator<<(std::ostream& out, const Move& move);