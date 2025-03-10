#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

typedef uint64_t u64;
typedef int16_t i16;
typedef uint16_t u16;

void	run();
void	printBitboard(u64 bitboard);
u64		generateRookMoves(u16 rookPosition);