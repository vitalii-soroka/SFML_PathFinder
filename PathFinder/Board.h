#pragma once
#include <array>
#include <iostream>
#include <assert.h>
#include "SFML/Graphics.hpp"

using u_short = unsigned short;

template <typename value_type = u_short>
using position = std::pair<value_type, value_type>;

template <typename value_type = position<>>
using Array = std::vector<std::vector<value_type>>;

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
	Board(position<> origin, sf::Vector2f cellSize, u_short collumns, u_short rows);
	
	void draw(sf::RenderWindow& window);
	void clear();
	void reset();

	void setStart(u_short i, u_short j);
	void setFinish(u_short i, u_short j);
	void setState(position<> pos, Cell state); 
	void setState(u_short i, u_short j, Cell state);

	void copyCells(const Board* board);

	/***************
	* Cell Position, Dimension
	****************/
	Cell atPos(position<> pos) const;
	Cell at(u_short i, u_short j) const;
	bool checkPos(position<> pos) const;

	Array<Cell> getCells() const;
	std::vector<position<>> getChilds(position<> parent) const;
	std::vector<position<>> getMiddleVerticles(position<> parent) const;
	std::vector<position<>> getDiagonalVerticles(position<> parent) const;

	sf::Vector2i getCellSize() const;

	position<> getStart() const;
	position<> getFinish() const;
	position<> getOrigin() const { return origin; }

	auto getRows() const { return p_array.size(); }
	auto getCollumns() const { return p_array[0].size(); }

	bool isSameCell(position<> a, position<> b) const;
private:
	position<> origin;
	indexes start;
	indexes finish;

	Array<Cell> p_array;
	sf::RectangleShape rect;

	// stores indexes of neighbours cells
	const int corners[4][2] = { {-1,-1}, {1,-1}, {-1,1}, {1,1} };
	const int mid[4][2] = { {-1,0},  {0, -1}, {0, 1}, {1, 0} };

	// transfroms position on screen to indexes in map
	indexes transform(position<> pos) const;

	// checks if diagonal cell not blocked
	bool checkDiagonal(u_short x, u_short y, u_short corner_index) const;
};


inline sf::Vector2i Board::getCellSize() const 
{ 
	return { (int)rect.getSize().x, (int)rect.getSize().y };
}
inline void Board::setState(u_short i, u_short j, Cell state)
{ 
	p_array[i][j] = state; 
}
inline Cell Board::at(u_short i, u_short j) const
{ 
	return p_array[i][j]; 
}
inline indexes Board:: transform(position<> pos) const
{
	//
	return { std::floor((pos.first - origin.first) / rect.getSize().x),
		std::floor((pos.second - origin.second) / rect.getSize().y) };
}