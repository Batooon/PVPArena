//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_GAME_H
#define PVPARENA_GAME_H

#include "World.h"
#include "Soldier.h"
#include "ActiveEntity.h"
#include "Entity.h"
#include "Command.h"
#include "SceneNode.h"
#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

class Game
{
public:
	Game();

	void run();

private:
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void processEvents();
	void update(sf::Time deltaTime);
	void render();

	sf::RenderWindow window;
	World world;
	static const sf::Time TimePerFrame;
	bool isPause;
};

#endif //PVPARENA_GAME_H
