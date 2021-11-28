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
			if(isPause == false)
				update(TimePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	CommandQueue& commands=world.getCommandQueue();
	sf::Event event{};
	while(window.pollEvent(event))
	{
		player.handleEvent(event, commands);
		switch(event.type)
		{
			case sf::Event::LostFocus:
				isPause = true;
				break;
			case sf::Event::GainedFocus:
				isPause = true;
				break;
//			case sf::Event::KeyPressed:
//				handlePlayerInput(event.key.code, true);
//				break;
//			case sf::Event::KeyReleased:
//				handlePlayerInput(event.key.code, false);
//				break;
			case sf::Event::Closed:
				window.close();
				break;
		}
		player.handleInput(commands);
	}
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