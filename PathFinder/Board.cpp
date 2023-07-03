#include "Board.h"

Board::Board(position<> origin, sf::Vector2f cellSize, u_short collumns, u_short rows) :
	origin(origin), p_array(rows, std::vector<Cell>(collumns,Cell::Empty))
{
	assert(rows >= 3 && collumns >= 3);

	rect.setSize(cellSize);
	rect.setPosition({ 0,0 });
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(2);

	setStart(1, 1);
	setFinish(collumns - 2, rows - 2);
}

void Board::clear()
{
	for (auto& row : p_array)
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
	for (auto& row : p_array)
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
	for (unsigned short x = 0; x < p_array.size(); ++x)
	{
		auto currentX = origin.first + x * rect.getSize().x;

		for (unsigned short y = 0; y < p_array[x].size(); ++y)
		{
			rect.setPosition(currentX, origin.second + y * rect.getSize().y);

			Cell cell_type = p_array[x][y];

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
			case Cell::Invalid: rect.setFillColor(sf::Color::Black);
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
	return p_array[p.first][p.second];
}

Array<Cell> Board::getCells() const
{
	return Array<Cell>(p_array);
}

void Board::copyCells(const Board* board)
{
	p_array = board->p_array;
	start = board->start;
	finish = board->finish;
}

std::vector<position<>> Board::getChilds(position<> parent) const
{
	std::vector<position<>> childs;

	for (int i = 0; i < 4; ++i)
	{
		int cx = parent.first + corners[i][0] * getCellSize().x;
		int cy = parent.second + corners[i][1] * getCellSize().y;

		if (checkPos({ cx,cy }))
		{
			auto cell = atPos(position<>{ cx, cy });

			if ((cell == Cell::Empty || cell == Cell::Finish) && checkDiagonal(cx, cy, i))
			{
				childs.push_back({ cx,cy });
			}
		}

		int mx = parent.first + mid[i][0] * getCellSize().x;
		int my = parent.second + mid[i][1] * getCellSize().y;

		if (checkPos({ mx,my }))
		{
			auto cell = atPos(position<>{ mx, my });

			if (cell == Cell::Empty || cell == Cell::Finish)
			{
				childs.push_back({ mx,my });
			}
		}
	}
	return childs;
}

std::vector<position<>> Board::getMiddleVerticles(position<> parent) const
{
	std::vector<position<>> childs;

	for (int i = 0; i < 4; ++i)
	{
		int x = parent.first + mid[i][0] * (int)getCellSize().x;
		int y = parent.second + mid[i][1] * (int)getCellSize().y;

		if (checkPos({ x,y })) {

			auto cell = atPos(position<>{ x, y });

			if (cell == Cell::Empty || cell == Cell::Finish)
			{
				childs.push_back({ x,y });
			}
		}
	}
	return childs;
}

std::vector<position<>> Board::getDiagonalVerticles(position<> parent) const 
{
	std::vector<position<>> childs;

	for (int i = 0; i < 4; ++i)
	{
		int x = parent.first + corners[i][0] * getCellSize().x;
		int y = parent.second + corners[i][1] * getCellSize().y;

		if (checkPos({ x,y })) {

			auto cell = atPos(position<>{ x, y });

			if (cell == Cell::Empty || cell == Cell::Finish)
			{
				switch (i)
				{
				case 0: if (atPos({ x + getCellSize().x, y }) == Cell::Wall &&
					atPos({ x, y + getCellSize().y }) == Cell::Wall) continue;
					break;
				case 1:	if (atPos({ x - getCellSize().x , y }) == Cell::Wall &&
					atPos({ x, y + getCellSize().y }) == Cell::Wall) continue;
					break;
				case 2: if (atPos({ x, y - getCellSize().y }) == Cell::Wall &&
					atPos({ x + getCellSize().x, y }) == Cell::Wall) continue;
					break;
				case 3: if (atPos({ x - getCellSize().x, y }) == Cell::Wall &&
					atPos({ x, y - getCellSize().y }) == Cell::Wall) continue;
					break;
				default:
					break;
				}
				childs.push_back({ x,y });
			}
		}
	}
	return childs;
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

void Board::setState(position<> pos, Cell state)
{
	auto p = transform(pos);
	p_array[p.first][p.second] = state;
}

bool Board::checkPos(position<> pos) const
{
	auto p = transform(pos);

	return /*p.first >= 0 && */ p.first < p_array.size() &&
		/*p.second >= 0 && */ p.second < p_array[0].size()
		? true : false;
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

void Board::setFinish(u_short x, u_short y)
{
	if (p_array[x][y] != Cell::Start)
	{
		p_array[finish.first][finish.second] = Cell::Empty;
		p_array[x][y] = Cell::Finish;
		finish = { x,y };
	}
}

void Board::setStart(u_short x, u_short y)
{
	if (p_array[x][y] != Cell::Finish) 
	{
		p_array[start.first][start.second] = Cell::Empty;
		p_array[x][y] = Cell::Start;
		start = { x,y };
	}
}
