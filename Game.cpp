//
// Created by Anton on 28/11/2021.
//
#include "Game.h"

Game::Game() :
		window(sf::VideoMode(1920, 1080), "PvP Arena", sf::Style::Close),
		world(window),
		player()
{
}

void Game::run()
{
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while(window.isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		while(timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	CommandQueue &commands = world.getCommandQueue();
	sf::Event event{};
	while(window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			window.close();
	}
	player.handleInput(commands);
}

void Game::update(sf::Time deltaTime)
{
	world.update(deltaTime);
}

void Game::render()
{
	window.clear();
	world.draw();
	window.display();
}