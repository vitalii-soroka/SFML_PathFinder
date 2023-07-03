#pragma once
#include "SearchAlgorithm.h"

/**
* Dijkstra’s Search algorithm,
* uses
*/
class DijkstraSearch : public SearchAlgorithm
{
public:
	using SearchAlgorithm::SearchAlgorithm;
	search_result_t search(position<> start, position<> end, const Board& board) override;
};
