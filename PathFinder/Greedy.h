#pragma once
#include "SearchAlgorithm.h"

/**
* Greedy Search algorithm,
* use distance from current cell to finish to select best next step
*/
class GreedySearch : public SearchAlgorithm
{
public:
	using SearchAlgorithm::SearchAlgorithm;
	search_result_t search(position<> start, position<> end, const Board& board) override;
};
