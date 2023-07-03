#include "AStar.h"
#include <queue>
#include <set>

bool ANodeComparator::operator()(ANode a, ANode b) const
{
    // Compares F = G + H values
    // if they are equal compares by G value (closer to start) 
    if ((a.G + a.H) == (b.G + b.H)) return a.G > b.G;
    return a.G + a.H > b.G + b.H;
}

search_result_t AStarSearch::search(position<> start, position<> end, const Board& board)
{
    // As algorithm uses set for visited for faster find etc,
    // for results another array used
    pos_arr visited_result;
    visited_result.reserve(board.getRows() * board.getCollumns());
    stats.reset(); // resets old values from previous call

    sf::Clock clock; // calculates elapsed time 

    p_map links;                        // link to parent cell
    std::map<position<>, int> f_values; // stores F values of each cell
    std::set<position<>> visited;       // closed 
    std::priority_queue<ANode, std::vector<ANode>, ANodeComparator> queue; // open

    // initialise start to queue
    f_values[start] = euclidean_heuristic(start, end);
    queue.emplace(start, 0, f_values[start]);
    
    // look for end until there is cells to check
    while(!queue.empty())
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

        // cell isn't a finish so add to close set
        VISITED
        visited.insert(current.pos);
        visited_result.push_back(current.pos);

        // check all neighbours cells of current cell
        for (const auto& child : board.getChilds(current.pos))
        {
            CHECK
            // cell have been already visited
            if (visited.find(child) != visited.end()) continue;

            // calculates new G,H and F values
            int newG = current.G + euclidean_heuristic(current.pos, child);
            int newH = manhattan_heuristic(child, end);
            int newF = newG + newH;

            CHECK
            // Adds cell to open queue if it doesn't have f value 
            // or it bigger than new - it's essential to update for shortest path
            if (f_values.find(child) == f_values.end() || newF < f_values[child])
            {
                links[child] = current.pos;
                f_values[child] = newF;
                queue.emplace(child, newG, newH);
            }
        }
    }
    TIME
    return { visited_result,{} };
}
