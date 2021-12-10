//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_ACTIVEENTITY_H
#define PVPARENA_ACTIVEENTITY_H

#include "Entity.h"
#include "Command.h"
#include "SceneNode.h"
#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

class ActiveEntity : public Entity
{
public:
	explicit ActiveEntity(const sf::Texture& texture, const int health);
	ActiveEntity(const sf::Texture& texture, const int health, const sf::IntRect& rect);
	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float x, float y);
	sf::Vector2f getVelocity() const;
	void accelerate(sf::Vector2f velocity);
	void dealDamage(int hp);
	void heal(int hp);
	bool isDead();

protected:
	int health;

private:
	virtual void updateCurrent(sf::Time deltaTime) override;
	sf::Vector2f velocity;
};

#endif //PVPARENA_ACTIVEENTITY_H
