//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_SOLDIER_H
#define PVPARENA_SOLDIER_H

#include "Entity.h"
#include "Projectile.h"
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
#include "Utilities.h"
#include <iostream>

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
	void Fire();
	void checkFire(sf::Time deltaTime, CommandQueue& commands);
	sf::Vector2f getFacingDirection() const;
private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateCurrent(sf::Time deltaTime, CommandQueue& commands) override;
	void SpawnBullet(SceneNode& node, const TextureHolder& textureHolder) const;
	void CreateProjectile(SceneNode& node, Projectile::Type type, float offsetx, float offsety, const TextureHolder&
	textureHolder) const;
	Type playerType;
	sf::Sprite sprite;
	bool isFiring;
	sf::Time fireCountdown;
	Command fireCommand;
	int fireRate;
	sf::Vector2f facingNormalized;
};

#endif //PVPARENA_SOLDIER_H
