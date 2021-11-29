#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include "ResourceHolder.h"
#include "SceneNode.h"
#include "Entity.h"
#include "ActiveEntity.h"
#include "Soldier.h"
#include "World.h"
#include "Game.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f/120.f);

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch(std::exception& e)
	{
		std::cout<<"Exception: "<<e.what()<<std::endl;
	}

	return 0;
}
