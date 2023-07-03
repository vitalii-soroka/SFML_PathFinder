#pragma once
#include <map>
#include <queue>
#include <list>
#include <utility>
#include "Board.h"
#include "Observer.h"
#include "SFML/Graphics/RenderWindow.hpp"

/** defines stats just to simplify algorithm readability */
#define CHECK ++stats.checks;
#define VISITED ++stats.visited;
#define PATH ++stats.pathLength;
#define TIME stats.time = clock.getElapsedTime().asMilliseconds();

using pos_arr = std::vector<position<>>;
using p_map = std::map<position<>, position<>>;	
using search_result_t = std::pair < pos_arr, pos_arr>;

/**
* Simple Node with location and value
*/
struct Node
{
	Node(position<> pos, int value) : pos(pos), value(value) {}
	position<> pos;
	int value;
};

/**
* Custom comparator for a node
*/
class NodeComparator
{
public:
	bool operator()(Node a, Node b) const
	{
		return (a.value) > (b.value);
	}
};

/**
* Struct to hold some stats,
* it is so primitive, because there is no much of them
*/
struct SearchStats
{
	int checks = 0;
	int visited = 0;
	int pathLength = 0;
	int time = 0;

	void reset() { visited = 0; pathLength = 0; checks = 0;  time = 0; }
};

/**
* Abstract class of search algorithm
*/
class SearchAlgorithm : public ISubject
{
public:
	~SearchAlgorithm() override = default;

	// Interface - all search algorithms should provide definitions
	virtual search_result_t search(position<> start, position<> end, const Board& board) = 0;

	// Default implementations 
    pos_arr createPath(position<> start, position<> finish, const p_map& map);

	// There are several heuristic formulas
	// Manhattan - difference between coordinates
	int manhattan_heuristic(position<> a, position<> b) const;
	// Euclidean - the distance between two positions
	int euclidean_heuristic(position<> a, position<> b) const;
	int octile_heuristic(position<> a, position<> b) const;
	
	// observer
	void attach(IObserver* observer) override;
	void detach(IObserver* observer) override;
	void notify() override;

	SearchStats getStats() const  { return stats; }

protected:
	SearchStats stats;

private:
	std::list<IObserver*> observers;
};

inline int SearchAlgorithm::euclidean_heuristic(position<> a, position<> b) const
{
	return (int)std::sqrt(std::pow(a.first - b.first, 2) + std::pow(a.second - b.second, 2));
}
inline void SearchAlgorithm::attach(IObserver* observer) 
{ 
	observers.push_back(observer);
}
inline void SearchAlgorithm::detach(IObserver* observer) 
{ 
	observers.remove(observer);
}
inline void SearchAlgorithm::notify() 
{ 
	for (auto& observer : observers) observer->update(); 
}
