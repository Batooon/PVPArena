//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_SOLDIER_H
#define PVPARENA_SOLDIER_H

#include "Entity.h"
#include "Command.h"
#include "SceneNode.h"
#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Data.h"
#include <vector>

class Soldier : public Entity
{
public:
	enum Type
	{
		DoubleGun,
		LaserGun,
		MagmaGun,
		Count
	};

	Soldier(Type type, const TextureHolder& textureHolder);

	void LookAt(sf::Vector2f worldPosition);
	unsigned int getCategory() const override;
	float getSpeed();
	sf::FloatRect getBounds() const override;
private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateCurrent(sf::Time deltaTime, CommandQueue& commands) override;
	Type playerType;
	sf::Sprite sprite;
};

#endif //PVPARENA_SOLDIER_H
