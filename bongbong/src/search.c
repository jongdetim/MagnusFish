#include "search.h"
#include "evaluate.h"
#include "generate.h"

#include <limits.h>

struct search_result minimax(const struct position *pos, int depth) {
	struct search_result result;

	result.score = -1000000;

	if (depth == 0) {
		/* we have reached our search depth, so evaluate the position.       */
		result.score = evaluate(pos);
	} else {
		struct move moves[MAX_MOVES];
		size_t count = generate_legal_moves(pos, moves);
		size_t index;

		for (index = 0; index < count; index++) {
			struct position copy = *pos;
			int score;

			/* do a move, the current player in `copy` is then the opponent, */
			/* and so when we call minimax we get the score of the opponent. */
			do_move(&copy, moves[index]);

			/* minimax is called recursively. this call returns the score of */
			/* the opponent, so we must negate it to get our score.          */
			score = -minimax(&copy, depth - 1).score;

			/* update the best move if we found a better one.                */
			if (score > result.score) {
				result.move = moves[index];
				result.score = score;
			}
		}
	}

	return result;
}

#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static struct move	specialSauce(const struct search_info *info, int movecount)
{
	struct move bongcloud = {0, 0, NO_TYPE};

	if (info->pos->side_to_move == WHITE)
	{
		switch (movecount)
		{
			case 1:
				bongcloud.from_square = SQUARE(FILE_E, RANK_2);
				bongcloud.to_square = SQUARE(FILE_E, RANK_3);
				break;
			case 2:
				bongcloud.from_square = SQUARE(FILE_E, RANK_1);
				bongcloud.to_square = SQUARE(FILE_E, RANK_2);
				break;
			case 3:
				bongcloud.from_square = SQUARE(FILE_E, RANK_2);
				bongcloud.to_square = SQUARE(FILE_E, RANK_1);
				break;
			default:
				break;
		}
	}
	else if (info->pos->side_to_move == BLACK)
	{
		switch (movecount)
		{
			case 1:
				bongcloud.from_square = SQUARE(FILE_E, RANK_7);
				bongcloud.to_square = SQUARE(FILE_E, RANK_6);
				break;
			case 2:
				bongcloud.from_square = SQUARE(FILE_E, RANK_8);
				bongcloud.to_square = SQUARE(FILE_E, RANK_7);
				break;
			case 3:
				bongcloud.from_square = SQUARE(FILE_E, RANK_7);
				bongcloud.to_square = SQUARE(FILE_E, RANK_8);
				break;
			default:
				break;
		}
	}
	if (is_legal(info->pos, bongcloud) == 0)
	{
		sleep(50);
		return (minimax(info->pos, 4).move);
	}
	sleep(5);
	return (bongcloud);
}

struct move search(const struct search_info *info)
{
	static int movecount = 0;

	movecount++;
	if (movecount < 4)
		return specialSauce(info, movecount);
	return minimax(info->pos, 4).move;
}
