#include "Board.h"

Board::Board(const sf::Vector2i& origin, const sf::Vector2i& grid_res, const sf::Vector2i& cellSize, float scale) :
	origin(origin.x,origin.y), grid(grid_res.x, std::vector<Cell>(grid_res.y,Cell::Empty))
{
	assert(grid_res.x >= 3 && grid_res.y >= 3);

	rect.setSize({(float)cellSize.x * scale, (float)cellSize.y * scale });
	rect.setPosition({ 0,0 });
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(2);

	setStart(1, 1);
	setFinish(grid_res.x - 2, grid_res.y - 2);
}

void Board::clear()
{
	for (auto& row : grid)
	{
		for (auto& element : row)
		{
			if (element != Cell::Finish && element != Cell::Start)
			{
				element = Cell::Empty;
			}
		}
	}
}

void Board::reset()
{
	for (auto& row : grid)
	{
		for (auto& element : row)
		{
			if (element == Cell::Visited || element == Cell::Path)
			{
				element = Cell::Empty;
			}
		}
	}
}

void Board::draw(sf::RenderWindow& window)
{
	for (unsigned short x = 0; x < grid.size(); ++x)
	{
		auto currentX = origin.first + x * rect.getSize().x;

		for (unsigned short y = 0; y < grid[x].size(); ++y)
		{
			rect.setPosition(currentX, origin.second + y * rect.getSize().y);

			Cell cell_type = grid[x][y];

			switch (cell_type)
			{
			case Cell::Empty: rect.setFillColor(sf::Color(36, 36, 85));
				break;
			case Cell::Wall:  rect.setFillColor(sf::Color::Black);
				break;
			case Cell::Visited: rect.setFillColor(sf::Color(36, 36, 145));
				break;
			case Cell::Path: rect.setFillColor(sf::Color::Magenta);
				break;
			case Cell::Invalid: rect.setFillColor(sf::Color::Yellow);
				break;
			case Cell::Finish: rect.setFillColor(sf::Color::Red);
				break;
			case Cell::Start: rect.setFillColor(sf::Color::Green);
				break;
			default:
				break;
			}
			window.draw(rect);
		}
	}
}

Cell Board::atPos(position<> pos) const
{
	auto p = transform(pos);
	return grid[p.first][p.second];
}

Array<Cell> Board::getCells() const
{
	return Array<Cell>(grid);
}

std::vector<position<>> Board::getChilds(position<> parent) const
{
	std::vector<position<>> childs;

	for (int i = 0; i < 4; ++i)
	{
		// corner indexes
		int x = parent.first + corners[i][0] * getCellSize().x;
		int y = parent.second + corners[i][1] * getCellSize().y;

		if (checkPos({ x,y }))
		{
			auto cell = atPos(position<>{ x, y });

			if ((cell == Cell::Empty || cell == Cell::Finish) && checkDiagonal(x, y, i))
			{
				childs.push_back({ x,y });
			}
		}

		// middle indexes
		x = parent.first + mid[i][0] * getCellSize().x;
		y = parent.second + mid[i][1] * getCellSize().y;

		if (checkPos({ x,y }))
		{
			auto cell = atPos(position<>{ x, y });

			if (cell == Cell::Empty || cell == Cell::Finish)
			{
				childs.push_back({ x,y });
			}
		}
	}
	return childs;
}

position<> Board::getStart() const
{
	return position<>
	{
		origin.first + start.first * (int)rect.getSize().x,
			origin.second + start.second * (int)rect.getSize().y,
	};
}

position<> Board::getFinish() const
{
	return position<>
	{
		origin.first + finish.first * (int)rect.getSize().x,
			origin.second + finish.second * (int)rect.getSize().y,
	};
}

void Board::setStart(u_short x, u_short y)
{
	if (grid[x][y] != Cell::Finish) 
	{
		grid[start.first][start.second] = Cell::Empty;
		grid[x][y] = Cell::Start;
		start = { x,y };
	}
}

void Board::setFinish(u_short x, u_short y)
{
	if (grid[x][y] != Cell::Start)
	{
		grid[finish.first][finish.second] = Cell::Empty;
		grid[x][y] = Cell::Finish;
		finish = { x,y };
	}
}

void Board::copyCells(const Board* board)
{
	grid = board->grid;
	start = board->start;
	finish = board->finish;
}

void Board::setCell(position<> pos, Cell state)
{
	if (state == Cell::Start || state == Cell::Finish) return;

	auto p = transform(pos);

	if (grid[p.first][p.second] == Cell::Start || 
		grid[p.first][p.second] == Cell::Finish) return;

	grid[p.first][p.second] = state;
}

bool Board::checkDiagonal(u_short cx, u_short cy, u_short corner_index) const
{
	switch (corner_index)
	{
	case 0: if (atPos({ cx + getCellSize().x, cy }) == Cell::Wall &&
		atPos({ cx, cy + getCellSize().y }) == Cell::Wall) return false;
		break;
	case 1:	if (atPos({ cx - getCellSize().x , cy }) == Cell::Wall &&
		atPos({ cx, cy + getCellSize().y }) == Cell::Wall) return false;
		break;
	case 2: if (atPos({ cx, cy - getCellSize().y }) == Cell::Wall &&
		atPos({ cx + getCellSize().x, cy }) == Cell::Wall) return false;
		break;
	case 3: if (atPos({ cx - getCellSize().x, cy }) == Cell::Wall &&
		atPos({ cx, cy - getCellSize().y }) == Cell::Wall) return false;
		break;
	default:
		break;
	}
	return true;
}

bool Board::isSameCell(position<> a, position<> b) const
{
	return transform(a) == transform(b);
}

bool Board::checkPos(position<> pos) const
{
	auto p = transform(pos);

	return p.first < grid.size() &&
		p.second < grid[0].size()
		? true : false;
}
