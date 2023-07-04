#pragma once
#include <queue>
#include <array>
#include <chrono>
#include <thread>
#include <iostream>
#include "Board.h"
#include "BoardHandler.h"
#include "ScreenManager.h"
#include <SFML/Graphics.hpp>

// Algorithms
#include "BFS.h"
#include "AStar.h"
#include "Dijkstra.h"
#include "Greedy.h"

namespace properties
{
	const sf::Vector2i origin{ 30, 30 };
	const sf::Vector2i cell_res{ 15, 15 };
	const sf::Vector2i grid_res{ 30, 30 };
	const sf::Vector2i screen_res{ 1920, 1080 };
	const int fps(60);
}

using namespace sf;
using namespace properties;

int main()
{
	Event event;
	RenderWindow window(VideoMode(screen_res.x, screen_res.y), "Pathfinder", Style::Close);
	window.setView(View(FloatRect(0, 0, (float)screen_res.x, (float)screen_res.y)));
	window.setFramerateLimit(fps);

	ScreenManager screen_manager(window);

	int boardSizeX = grid_res.x * cell_res.x;
	int boardSizeY = grid_res.y * cell_res.y;

	// offset from board X and Y
	int offsetX = 2 * boardSizeX;
	int offsetY = boardSizeY + 2 * cell_res.y;

	int boxOffsetX = boardSizeX + 2 * cell_res.x;
	int boxOffsetY = boardSizeY + 2 * cell_res.y;

	// Boards initialisation
	Board bfsBoard({30,30}, grid_res, cell_res);
	Board dijkstraBoard({ origin.x, origin.y + offsetY }, grid_res, cell_res);
	Board greedyboard({ origin.x + offsetX, origin.y }, grid_res, cell_res);
	Board aStarBoard({ origin.x + offsetX, origin.y + offsetY }, grid_res, cell_res);

	// Search algorithms
	BFSearch bfs;
	DijkstraSearch dijkstraSearch;
	GreedySearch greedySearch;
	AStarSearch aStarSearch;

	// Display search details
	StatsBox BfsStats("BFS", { origin.x + boxOffsetX , origin.y });
	StatsBox DiklstraStats("Dijkstra", { origin.x + boxOffsetX, origin.y + boxOffsetY });
	StatsBox GreedyStats("Greedy", { origin.x + 2 * boardSizeX + boxOffsetX, origin.y });
	StatsBox AStarStats("A*", { origin.x + 2 * boardSizeX + boxOffsetX, origin.y + boxOffsetY });

	// Handlers for board, algorithm and stats
	BoardHandler bfsHandler(bfsBoard, &bfs, BfsStats);
	BoardHandler dijkstraHandler(dijkstraBoard, &dijkstraSearch, DiklstraStats);
	BoardHandler greedyHandler(greedyboard,&greedySearch, GreedyStats);
	BoardHandler aStartHandler(aStarBoard, &aStarSearch, AStarStats);
	
	screen_manager.addHandler(&bfsHandler);
	screen_manager.addHandler(&dijkstraHandler);
	screen_manager.addHandler(&greedyHandler);
	screen_manager.addHandler(&aStartHandler);

	// Main loop
    while (window.isOpen())
	{
		while (window.pollEvent(event))
		{ 
			screen_manager.handleInput(event);
		}

		window.clear();
		screen_manager.draw();
		window.display();
	}
	return 0;
}

