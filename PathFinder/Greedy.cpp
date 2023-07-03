#include "Greedy.h"
#include <queue>
#include <set>

search_result_t GreedySearch::search(position<> start, position<> end, const Board& board)
{
    // As algorithm uses set for visited for faster find etc,
    // for results another array used
    pos_arr visited_result;
    visited_result.reserve(board.getRows() * board.getCollumns());
    stats.reset();  // resets old values from previous call
    
    sf::Clock clock;    // calculates elapsed time 
    
    p_map links;                            //
    std::map<position<>, int> distance;     // cell distances 
    std::set<position<>> visited;           // closed
    std::priority_queue<Node, std::vector<Node>, NodeComparator> queue; // open

    // initialise start to queue
    distance[start] = euclidean_heuristic(start,end);
    queue.emplace(start, 0);

    // look for end until there is cells to check
    while (!queue.empty())
    {
        // take smallest out of open queue
        auto current = queue.top();
        queue.pop();

        CHECK
        if (current.pos == end)
        {
            TIME
            return  { visited_result, createPath(start, current.pos, links) };
        }

        VISITED
        // cell isn't a finish so add to close set
        visited.insert(current.pos);
        visited_result.push_back(current.pos);

        // checks all neighbours cells to current cell
        for (const auto& child : board.getChilds(current.pos))
        {
            CHECK
            // cell already in close set
            if (visited.find(child) != visited.end()) continue;

            // distance to finish
            auto new_distance = euclidean_heuristic(child, end);

            CHECK
            // Adds cell to open queue if it doesn't have f value 
            // or it bigger than new distance
            if (distance.find(child) == distance.end() || new_distance < distance[child])
            {
                links[child] = current.pos;
                distance[child] = new_distance;
                queue.emplace(child, distance[child]);
            }
        }
    }
    TIME
    return { visited_result, {} };
}
