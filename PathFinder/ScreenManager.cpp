#include "ScreenManager.h"

ScreenManager::ScreenManager(sf::RenderWindow& window)
	: window(window)
{
}

void ScreenManager::draw() const
{
	for (auto& board : boards_handlers)
	{
		board->draw(window);
	}
}

void ScreenManager::handleInput(const sf::Event& event)
{
	if (event.type == sf::Event::Closed) {
		window.close();
	}

	for (auto& board : boards_handlers) 
	{
		if (board->mouseOnBoard(window)) 
		{
			board->handleInput(event, window);
			break;
		}
	}
}

void ScreenManager::addHandler(BoardHandler* board)
{
	boards_handlers.push_back(board);
	board->attach(this);
}

void ScreenManager::update()
{
	static unsigned short i = 1;
	if (updateRate % i == 0)
	{
		i = 1;
		window.clear();
		draw();
		window.display();
	}
}
