//
// Created by Anton on 28/11/2021.
//
#include "Entity.h"

//Entity::Entity(const sf::Texture& texture) : sprite(texture)
//{
//}
//
//Entity::Entity(const sf::Texture& texture, const sf::IntRect& rect) : sprite(texture, rect)
//{
//}
//
//void Entity::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
//{
//	target.draw(sprite, states);
//}
//
//sf::FloatRect Entity::getLocalBounds() const
//{
//	return sprite.getLocalBounds();
//}
//
//float Entity:getMaxSpeed() const
//{
//	return speed;
//};

Entity::Entity(int hp):health(hp), velocity()
{
}

void Entity::SetVelocity(sf::Vector2f velocity)
{
	velocity = velocity;
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
	velocity.x+=x;
	velocity.y+=y;
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

bool Entity::isDead() const
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
