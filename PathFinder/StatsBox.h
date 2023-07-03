#pragma once
#include <set>
#include <map>
#include <functional>
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SearchAlgorithm.h"

/**
 * Simple class to visualise algorithm statistic on screen
 */
class StatsBox
{
public:
	StatsBox(const std::string& name, position<> pos);

	void update(const SearchStats& stats);
	void draw(sf::RenderWindow& window) const;

private:
	// distance betweens lines
	float offset = 20;
	sf::Font font;
	std::vector<std::string> titles;
	std::map<std::string, sf::Text, std::less<>> texts;
};