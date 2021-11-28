#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include "ResourceHolder.h"
#include "SceneNode.h"
#include "Entity.h"
#include "ActiveEntity.h"
#include "Player.h"
#include "World.h"
#include "Game.h"

template<class VectorT>
sf::Vector2f Normalize(VectorT vector)
{
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	if(length != 0)
		return sf::Vector2f(vector.x / length, vector.y / length);
	return (sf::Vector2f)vector;
}

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
