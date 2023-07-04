#include "BoardHandler.h"

Board* BoardHandler::cache = nullptr;

BoardHandler::BoardHandler(Board& board, SearchAlgorithm* algorithm, StatsBox& statsBox)
	: board(board), algorithm(algorithm), statBox(statsBox)
{

}

void BoardHandler::draw(sf::RenderWindow& window) const
{
	board.draw(window);
	statBox.draw(window);
}

void BoardHandler::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
	// Just primitive switch for several inputs, command design can be used
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed : 
		handleMouseClick(event, window);
		break;
	case sf::Event::MouseMoved :
		handleMouseMoved(event, window);
		break;
	case sf::Event::KeyPressed:
		handleButtonsClick(event, window);
		break;
	}
}

bool BoardHandler::mouseOnBoard(const sf::RenderWindow& window) const
{
	float ORIGIN_X = board.getOrigin().first;
	float ORIGIN_Y = board.getOrigin().second;

	auto CELL_SIZE = board.getCellSize();

	auto mouse_x = (float)sf::Mouse::getPosition(window).x;
	auto mouse_y = (float)sf::Mouse::getPosition(window).y;

	return  mouse_x > ORIGIN_X && mouse_x < ORIGIN_X + (float)(CELL_SIZE.x * board.getCollumns()) &&
		mouse_y > ORIGIN_Y && mouse_y < ORIGIN_Y + (float)(CELL_SIZE.y * board.getRows());
}

void BoardHandler::visualise_algorithm(const search_result_t& result)
{
	// Drawing visited cells
	for (auto it = result.first.begin() + 1; it != result.first.end(); ++it)
	{
		board.setCell(*it, Cell::Visited);
		notify();
	}

	if (result.second.empty()) return;

	// Drawing path cells
	for (auto it = result.second.begin() + 1; it != result.second.end(); ++it)
	{
		board.setCell(*it, Cell::Path);
		notify();
	}
}

position<short> BoardHandler::get_mouse_cell(const sf::RenderWindow& window) const
{
	float ORIGIN_X = board.getOrigin().first;
	float ORIGIN_Y = board.getOrigin().second;

	auto CELL_SIZE = board.getCellSize();

	float mouse_x = (float)sf::Mouse::getPosition(window).x - ORIGIN_X;
	float mouse_y = (float)sf::Mouse::getPosition(window).y - ORIGIN_Y;

	return position<short>(std::floor(mouse_x / (float)CELL_SIZE.x),
		std::floor(mouse_y / (float)CELL_SIZE.y));
}

void BoardHandler::drawLine(sf::Vector2i start, sf::Vector2i end, Cell type) 
{
	// Bresenham's line algorithm can be used,
	// but line slope and line formula is used 
	
	int dx = end.x - start.x;
	int dy = end.y - start.y;

	int steps = std::max(std::abs(dx) / ( board.getCellSize().x / 2),
		std::abs(dy) / (board.getCellSize().y / 2 ) );

	float xIncrement = (float)(dx) / steps;
	float yIncrement = (float)(dy) / steps;

	auto x = (float)(start.x);
	auto y = (float)(start.y);

	for (int i = 0; i <= steps; ++i)
	{
		board.setCell({ (int)x, (int)y }, type);
		x += xIncrement;
		y += yIncrement;
	}
}

void BoardHandler::handleMouseClick(const sf::Event& event, const sf::RenderWindow& window)
{
	auto indx = get_mouse_cell(window);
	auto cell = board.at(indx.first, indx.second);

	if (cell != Cell::Start && cell != Cell::Finish)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			board.setCell(indx.first, indx.second, Cell::Wall);
			mouseButtonHold = true;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			board.setCell(indx.first, indx.second, Cell::Empty);
			mouseButtonHold = true;
		}
	}
	prevMousePos = sf::Mouse::getPosition(window);
}

void BoardHandler::handleMouseMoved(const sf::Event& event, const sf::RenderWindow& window)
{
	if (!mouseOnBoard(window) || !mouseButtonHold) return;
	   
	sf::Vector2i currentPos = sf::Mouse::getPosition(window);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!board.isSameCell({ prevMousePos.x,prevMousePos.y }, { currentPos.x,currentPos.y }))
		{
			drawLine(prevMousePos, currentPos, Cell::Wall);
			prevMousePos = currentPos;
		}

	}

	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (!board.isSameCell({ prevMousePos.x,prevMousePos.y }, { currentPos.x,currentPos.y }))
		{
			drawLine(prevMousePos, currentPos, Cell::Empty);
			prevMousePos = currentPos;
		}
	}
	else
	{
		mouseButtonHold = false;
		prevMousePos = currentPos;
	}
}

void BoardHandler::handleButtonsClick(const sf::Event& event, const sf::RenderWindow& window)
{
	// Very simple input handle
	switch (event.key.code)
	{
	case sf::Keyboard::F: // FIND: Find Path
	{
		board.reset();
		auto result = algorithm->search(board.getStart(), board.getFinish(), board);
		visualise_algorithm(result);
		statBox.update(algorithm->getStats());
		break;
	}
	case sf::Keyboard::R: // RESET: clears visited and path cells
		board.reset();
		break;

	case sf::Keyboard::S: // SET START: sets cell to start on which mouse 
	{
		auto pos = get_mouse_cell(window);
		board.setStart(pos.first, pos.second);
		break;
	}

	case sf::Keyboard::E: // SET END: sets cell to finish on which mouse 
	{
		auto pos = get_mouse_cell(window);
		board.setFinish(pos.first, pos.second);
		break;
	}

	case sf::Keyboard::C: // COPY
	{
		cache = &board;
		break;
	}

	case sf::Keyboard::V: // PASTE
	{
		if (cache) board.copyCells(cache);
		break;
	}
	
	case sf::Keyboard::Space: // CLEAR: sets all cells to empty
		board.clear();
		break;

	default:
		break;
	}
}

