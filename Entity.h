//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_ENTITY_H
#define PVPARENA_ENTITY_H

#include "Command.h"
#include "SceneNode.h"
#include "ResourceHolder.h"
#include <functional>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "CommandQueue.h"

class Entity : public SceneNode
{
public:
	explicit Entity(int hp);
	void SetVelocity(const sf::Vector2f& velocity);
	void SetVelocity(float x, float y);
	void accelerate(const sf::Vector2f& velocity);
	void accelerate(float x, float y);
	sf::Vector2f getVelocity() const;
	void dealDamage(int hp);
	void heal(int hp);
	void Kill();
	virtual bool isDead() const;
	int getHp() const;

protected:
	void updateCurrent(sf::Time deltaTime, CommandQueue& commands) override;

private:
	sf::Vector2f velocity;
	int health;
};

#endif //PVPARENA_ENTITY_H
