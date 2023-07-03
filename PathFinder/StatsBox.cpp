#include "StatsBox.h"

StatsBox::StatsBox(const std::string& name, position<> pos)
	:
	titles {
	name,
	"Visited: ",
	"Path Length: ",
	"Checks: ",
	"Time: " 
	}
{
	font.loadFromFile("font/DS-DIGI.TTF");

	unsigned short i = 0;
	for (const auto& title : titles)
	{
		texts[title] = sf::Text();
		texts[title].setFont(font);
		texts[title].setCharacterSize(24);
		texts[title].setPosition(sf::Vector2f{ (float)pos.first, (float)pos.second + i * offset });
		texts[title].setString(title);
		++i;
	}
}

void StatsBox::draw(sf::RenderWindow& window) const
{
	for (const auto& x : texts)
	{
		window.draw(x.second);
	}
}

void StatsBox::update(const SearchStats& stats)
{
	// hardcoded as our stats struct primitive, and likely not be expanded
	texts[titles[1]].setString(titles[1] + std::to_string(stats.visited));
	texts[titles[2]].setString(titles[2] + std::to_string(stats.pathLength));
	texts[titles[3]].setString(titles[3] + std::to_string(stats.checks));
	texts[titles[4]].setString(titles[4] + std::to_string(stats.time) + " ms");
}

