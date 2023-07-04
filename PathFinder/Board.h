#pragma once
#include <array>
#include <iostream>
#include <assert.h>
#include "SFML/Graphics.hpp"

/*
* Alias
*/

using u_short = unsigned short;

// position storage
template <typename value_type = u_short>
using position = std::pair<value_type, value_type>;

// alias for two dimensional array of positions be default
template <typename value_type = position<>>
using Array = std::vector<std::vector<value_type>>;

// stores indexes
using indexes = std::pair<u_short, u_short>;

/**
* Primitive cell state 
*/
enum class Cell
{
	Empty,
	Wall,
	Visited,
	Path,
	Invalid,
	Start,
	Finish
};

/**
*  Board of cells
*/
class Board
{
public:
	Board(const sf::Vector2i& origin, const sf::Vector2i& grid_res, const sf::Vector2i& cellSize, float scale = 1.0f);

	void draw(sf::RenderWindow& window);
	// Removes visited and path cells, leaves walls
	void clear();
	// Fully clears board from all cells
	void reset();

	// Change cells to any state, except start or finish
	void setCell(position<> pos, Cell state); 
	void setCell(u_short i, u_short j, Cell state);

	void setStart(u_short i, u_short j);
	void setFinish(u_short i, u_short j);

	// copies all cells from other board
	void copyCells(const Board* board);

	/* Cell Position */
	Cell atPos(position<> pos) const;
	Cell at(u_short i, u_short j) const;

	/* Checks if there is cell on position */
	bool checkPos(position<> pos) const;
	
	/* Returns a copied array of all board cells */
	Array<Cell> getCells() const;
	
	sf::Vector2i getCellSize() const;

	/* Returns neighbours cells */ 
	std::vector<position<>> getChilds(position<> parent) const;
	std::vector<position<>> getMiddleVerticles(position<> parent) const;
	std::vector<position<>> getDiagonalVerticles(position<> parent) const;

	position<> getStart() const;
	position<> getFinish() const;
	position<> getOrigin() const { return origin; }

	// Dimension of board
	auto getRows() const { return grid.size(); }
	auto getCollumns() const { return grid[0].size(); }

	// Checks whenever two positions in bounds of same cell
	bool isSameCell(position<> a, position<> b) const;

private:
	position<> origin;
	indexes start;
	indexes finish;

	Array<Cell> grid;
	sf::RectangleShape rect;

	float scale;

	// stores indexes of neighbours cells
	const int corners[4][2] = { {-1,-1}, {1,-1}, {-1,1}, {1,1} };
	const int mid[4][2] = { {-1,0},  {0, -1}, {0, 1}, {1, 0} };

	// transfroms position on screen to indexes in board
	indexes transform(position<> pos) const;

	// checks if diagonal cell not blocked
	bool checkDiagonal(u_short x, u_short y, u_short corner_index) const;
};


inline sf::Vector2i Board::getCellSize() const 
{ 
	return { (int)(rect.getSize().x), (int)(rect.getSize().y) };
}
inline void Board::setCell(u_short i, u_short j, Cell state)
{ 
	grid[i][j] = state; 
}
inline Cell Board::at(u_short i, u_short j) const
{ 
	return grid[i][j]; 
}
inline indexes Board:: transform(position<> pos) const
{
	return { std::floor((pos.first - origin.first) / rect.getSize().x),
		std::floor((pos.second - origin.second) / rect.getSize().y) };
}