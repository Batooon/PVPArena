//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_SOLDIER_H
#define PVPARENA_SOLDIER_H

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
#include "Data.h"
#include <vector>

class Soldier : public ActiveEntity
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
private:
	Type playerType;
};

#endif //PVPARENA_SOLDIER_H
