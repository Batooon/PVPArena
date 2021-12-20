//
// Created by Anton on 28/11/2021.
//
#include "Entity.h"

Entity::Entity(int hp):health(hp), velocity()
{
}

void Entity::SetVelocity(const sf::Vector2f& velocity)
{
	this->velocity = velocity;
}

void Entity::SetVelocity(float x, float y)
{
	velocity.x=x;
	velocity.y=y;
}

void Entity::accelerate(const sf::Vector2f& velocity)
{
	this->velocity += velocity;
}

void Entity::accelerate(float x, float y)
{
	velocity.x += x;
	velocity.y += y;
}

sf::Vector2f Entity::getVelocity() const
{
	return velocity;
}

void Entity::dealDamage(int hp)
{
	health-=hp;
}

void Entity::heal(int hp)
{
	health+=hp;
}

bool Entity::isDestroyed() const
{
	return health<=0;
}

int Entity::getHp() const
{
	return health;
}

void Entity::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
	move(velocity * deltaTime.asSeconds());
}

void Entity::Kill()
{
	health = 0;
}
