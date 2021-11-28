//
// Created by Anton on 28/11/2021.
//
#include "Game.h"

Game::Game() :
		window(sf::VideoMode(1920, 1080), "PvP Arena", sf::Style::Close), world(window)
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

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
//	if(key == sf::Keyboard::W)
//		movingUp = isPressed;
//	else if(key == sf::Keyboard::S)
//		movingDown = isPressed;
//	else if(key == sf::Keyboard::A)
//		movingLeft = isPressed;
//	else if(key == sf::Keyboard::D)
//		movingRight = isPressed;
}

void Game::processEvents()
{
	sf::Event event{};
	while(window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::LostFocus:
				isPause = true;
				break;
			case sf::Event::GainedFocus:
				isPause = true;
				break;
			case sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;
			case sf::Event::Closed:
				window.close();
				break;
		}
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