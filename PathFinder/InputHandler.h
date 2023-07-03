#pragma once
#include "SFML/Graphics.hpp"


/**
* Interface for handling inputs
*/
class InputHandler
{
public:
	virtual ~InputHandler() = default;
	virtual void handleInput(const sf::Event& event, sf::RenderWindow& window) = 0;
};
