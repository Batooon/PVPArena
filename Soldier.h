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

class Soldier : public ActiveEntity
{
public:
	enum Type
	{
		DoubleGun,
		LaserGun,
		MagmaGun
	};

	Soldier(Type type, const TextureHolder& textureHolder);

	void LookAt(sf::Vector2f worldPosition);
	float getSpeed() const;
	unsigned int getCategory() const override;
private:
	const float speed = 250.f;
	Type playerType;
};

#endif //PVPARENA_SOLDIER_H
