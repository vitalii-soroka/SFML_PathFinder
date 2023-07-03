#include "SearchAlgorithm.h"

pos_arr SearchAlgorithm::createPath(position<> start, position<> finish, const p_map& map)
{
	pos_arr result;

	stats.pathLength = 0;

	auto cell = map.at(finish);

	result.push_back(cell);

	// Moves to parent cell by links 
	while (cell != start)
	{
		PATH
		//board.setState(cell, Cell::Path);
		cell = map.at(cell);
		result.push_back(cell);
	}
	std::reverse(result.begin(), result.end());
	return result;

	// make sure that finish and start not erased by visited
	//board.setState(start, Cell::Start);
	//board.setState(finish, Cell::Finish);
}

int SearchAlgorithm::manhattan_heuristic(position<> a, position<> b) const
{
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

int SearchAlgorithm::octile_heuristic(position<> a, position<> b) const
{
	int dx = abs(a.first - b.first);
	int dy = abs(a.second - b.second);
	return (int)(dx + dy - std::min(dx, dy) + std::sqrt(2) * std::min(dx, dy));
}
