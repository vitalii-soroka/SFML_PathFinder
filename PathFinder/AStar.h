#pragma once
#include "SearchAlgorithm.h"

/**
* A* algorithm provides own node.
*/
struct ANode
{
    ANode(position<> pos, int G, int H) : pos(pos), G(G), H(H) {}
    position<> pos;     // represents x,y position on screen.
    int G;              // represents path lenght from start.
    int H;              // represents heuristic value from node to end.
};

/**
* Special comparator for ANode
*/
class ANodeComparator
{
public:
    bool operator()(ANode a, ANode b) const;
};

/**
* A* (A star)  is a combination of Dijkstra and Greedy.
*/
static class AStarSearch : public SearchAlgorithm
{
public:
    using SearchAlgorithm::SearchAlgorithm;
    search_result_t search(position<> start, position<> end, const Board& board) override;
};
