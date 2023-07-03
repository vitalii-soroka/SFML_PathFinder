#pragma once
#include <vector>
#include "Observer.h"
#include "BoardHandler.h"
#include "SFML/Graphics.hpp"

/**
* Simple Screen Manager - just to draw boards and handle input
*/
class ScreenManager : public IObserver
{
public:
	explicit ScreenManager(sf::RenderWindow& window);

	ScreenManager(const ScreenManager&) = delete;
	ScreenManager& operator=(const ScreenManager&) = delete;

	// manager functionality
	void draw() const;
	void addHandler(BoardHandler* board);
	void handleInput(const sf::Event& event);

	// IObserver interface, called when board state changed
	void update() override;

private:
	// How often boards re-drawn from update calls
	unsigned short updateRate = 2;
	//
	sf::RenderWindow& window;
	std::vector<BoardHandler*> boards_handlers;
	// delegates check for board
	bool mouseOnBoard(const BoardHandler& board) const;
};

inline bool ScreenManager::mouseOnBoard(const BoardHandler& board) const
{
	return board.mouseOnBoard(window);
}