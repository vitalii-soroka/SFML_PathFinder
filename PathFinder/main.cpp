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

using namespace sf;

int main()
{
	Event event;

	RenderWindow window(VideoMode(1920, 1080), "Pathfinding", Style::Close);
	window.setView(View(FloatRect(0, 0, 1920, 1080)));

	window.setFramerateLimit(60);

	ScreenManager sm(window);

	Board bfsBoard({ 30,30 }, { 15,15 }, 30, 30);
	Board dijkstraBoard({ 30,510 }, { 15,15 }, 30, 30);
	Board greedyboard({ 720,30 }, { 15,15 }, 30, 30);
	Board aStarBoard({ 720,510 }, { 15,15 }, 30, 30);

	BFSearch bfs;
	DijkstraSearch dijkstraSearch;
	GreedySearch greedySearch;
	AStarSearch aStarSearch;

	StatsBox statsBox("BFS", { 510,30 });
	StatsBox statsBox2("Dijkstra", { 510,510 });
	StatsBox statsBox3("Greedy", { 1200,30 });
	StatsBox statsBox4("A*", { 1200,510 });

	BoardHandler bfsHandler(bfsBoard, &bfs, statsBox);
	BoardHandler dijkstraHandler(dijkstraBoard, &dijkstraSearch, statsBox2);
	BoardHandler greedyHandler(greedyboard,&greedySearch, statsBox3);
	BoardHandler aStartHandler(aStarBoard, &aStarSearch, statsBox4);
	
	sm.addHandler(&bfsHandler);
	sm.addHandler(&dijkstraHandler);
	sm.addHandler(&greedyHandler);
	sm.addHandler(&aStartHandler);

    while (window.isOpen())
	{
		while (window.pollEvent(event))
		{ 
			sm.handleInput(event);
		}

		window.clear();
		sm.draw();
		window.display();
	}

	return 0;
}

