#pragma once
#include "Board.h"
#include "SearchAlgorithm.h"

/**
* Breadth-First Search
*/
class BFSearch : public  SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;
    search_result_t search(position<> start, position<> end, const Board& board) override;
};
