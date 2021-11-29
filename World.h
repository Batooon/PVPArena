//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_WORLD_H
#define PVPARENA_WORLD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <functional>
#include "ResourceHolder.h"
#include "SceneNode.h"
#include "Command.h"
#include "Entity.h"
#include "ActiveEntity.h"
#include "Soldier.h"
#include "Resources.h"
#include "CommandQueue.h"

class World:sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);
	void update(sf::Time deltaTime);
	void draw();
	CommandQueue& getCommandQueue();

private:
	enum Layers
	{
		Background,
		Vegetation,
		Trees,
		Ground,
		LayerCount
	};

	void loadTextures();
	void buildScene();
	void clampWorldView();
	void clampPlayerPosition();
	sf::Vector2f getViewDeltaPosition(sf::Time deltaTime);

	sf::RenderWindow& window;
	sf::View worldView;
	TextureHolder textures;
	SceneNode sceneGraph;
	std::array<SceneNode*, LayerCount> sceneLayers;

	sf::FloatRect worldBounds;
	sf::Vector2f spawnPoint;
	Soldier* playerSoldier;
	CommandQueue commandQueue;
};

#endif //PVPARENA_WORLD_H
