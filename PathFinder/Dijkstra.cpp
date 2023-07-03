#include "Dijkstra.h"
#include <queue>
#include <set>

search_result_t DijkstraSearch::search(position<> start, position<> end, const Board& board)
{ 
    // As algorithm uses set for visited for faster find etc,
    // for results another array used
    pos_arr visited_result;
    visited_result.reserve(board.getRows() * board.getCollumns());
    stats.reset();  // resets old values from previous call

    sf::Clock clock; // calculates time 

    p_map links;                             //
    std::map<position<>, int> distance;      //
    std::set<position<>> visited;            // closed
     
    std::priority_queue<Node, std::vector<Node>, NodeComparator> queue; // open
    
    // initialise start to queue
    distance[start] = 0;
    queue.emplace(start,0);

    // look for end until there is cells to check
    while (!queue.empty())
    {
        // take out smallest F value node
        auto current = queue.top();
        queue.pop();

        CHECK
        if (current.pos == end)
        {
            TIME
            return { visited_result, createPath(start, current.pos, links) };
        }

        VISITED
        // cell isn't a finish so add to close set
        visited.insert(current.pos);
        visited_result.push_back(current.pos);

        // check all neighbours cells of current cell
        for (const auto& child : board.getChilds(current.pos))
        {
            CHECK
            // cell have been already visited
            if (visited.find(child) != visited.end()) continue;

            // calculates new distance
            int new_distance = distance[current.pos] + euclidean_heuristic(current.pos, child);

            CHECK
            // Adds cell to open queue if it doesn't have f value 
            // or it bigger than new
            if (distance.find(child) == distance.end() || new_distance < distance[child])
            {
                distance[child] = new_distance;
                links[child] = current.pos;
                queue.emplace(child, distance[child]);
            }
        }
    }
    TIME
    return { visited_result, {} };
}
