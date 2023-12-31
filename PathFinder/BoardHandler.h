#pragma once
#include "Board.h"
#include "StatsBox.h"
#include "InputHandler.h"
#include "SearchAlgorithm.h"
#include "SFML/System/Clock.hpp"

/**
* Wrapper for Board to handle it's behaviour
*/
class BoardHandler : public InputHandler, public ISubject
{
public:
	BoardHandler(Board& board,
		SearchAlgorithm* algorithm,
		StatsBox& statBox);

	// Functionality
	void draw(sf::RenderWindow& window) const;
	void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
	bool mouseOnBoard(const sf::RenderWindow& window) const;

	// Attach observer for algorithm
	void attachObserver(IObserver* observer) { algorithm->attach(observer); }
	
	// observer
	void attach(IObserver* observer) override;
	void detach(IObserver* observer) override;
	void notify() override;

private:
	// stores pointer to 'copied' board
	static Board* cache;
	// used to draw with mouse on board 
	sf::Vector2i prevMousePos;
	// references to handled data
	Board& board;
	SearchAlgorithm* algorithm;
	StatsBox& statBox;
	std::list<IObserver*> observers;

	position<short> get_mouse_cell(const sf::RenderWindow& window) const;

	void drawLine(sf::Vector2i first, sf::Vector2i second, Cell type);
	void visualise_algorithm(const search_result_t& result);
	void handleMouseClick(const sf::Event& event, const sf::RenderWindow& window);
	void handleMouseMoved(const sf::Event& event, const sf::RenderWindow& window);
	void handleButtonsClick(const sf::Event& event, const sf::RenderWindow& window);

	bool mouseButtonHold = false;
}; 

inline void BoardHandler::attach(IObserver* observer)
{
	observers.push_back(observer);
}

inline void BoardHandler::detach(IObserver* observer)
{
	observers.remove(observer);
}

inline void BoardHandler::notify()
{
	for (auto& observer : observers) observer->update();
}
