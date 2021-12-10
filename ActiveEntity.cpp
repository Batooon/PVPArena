//
// Created by Anton on 28/11/2021.
//
#include "ActiveEntity.h"

ActiveEntity::ActiveEntity(const sf::Texture& texture) : Entity(texture)
{
}

ActiveEntity::ActiveEntity(const sf::Texture& texture, const sf::IntRect& rect) : Entity(texture, rect)
{
}

void ActiveEntity::setVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}

void ActiveEntity::setVelocity(sf::Vector2f velocity)
{
	this->velocity.x = velocity.x;
	this->velocity.y = velocity.y;
}

sf::Vector2f ActiveEntity::getVelocity() const
{
	return velocity;
}

void ActiveEntity::updateCurrent(sf::Time deltaTime)
{
	move(velocity * deltaTime.asSeconds());
}

void ActiveEntity::accelerate(sf::Vector2f velocity)
{
	this->velocity += velocity;
}

void ActiveEntity::dealDamage(int hp)
{
	health -= hp;
}

void ActiveEntity::heal(int hp)
{
	health += hp;
}

bool ActiveEntity::isDead()
{
	return health <= 0;
}