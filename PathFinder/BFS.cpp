#include "BFS.h"
#include <queue>
#include <set>

search_result_t BFSearch::search(position<> start, position<> end, const Board& board)
{
    // As algorithm uses set for visited for faster find etc,
    // for results another array used
    pos_arr visited_result;
    visited_result.reserve(board.getRows() * board.getCollumns());
    stats.reset();  // resets old values from previous call

    sf::Clock clock; // calculates time 

    p_map links;                    // link to parent cell
    std::set<position<>> visited;   // closed set
    std::queue<position<>> queue;   // open queue

    // initialise start to queue
    queue.push(start);

    // looks for end until there is cells in open
    while (!queue.empty())
    {
        // take out smallest F value node
        auto current = queue.front();
        queue.pop();

        // check all neighbours cells of current cell
        for (const auto& child : board.getChilds(current))
        {
            CHECK
            if (child == end)
            {
                links[child] = current;
                TIME
                return  { visited_result, createPath(start, end, links) };
            }
           
            CHECK
            // set visited if not already
            if (visited.find(child) == visited.end()) 
            {
                VISITED
                queue.push(child);
                visited.insert(child);
                links[child] = current;
                visited_result.push_back(child);
            } 
        }
    }
    TIME
    return { visited_result, {} };
}
