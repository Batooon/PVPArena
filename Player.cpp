//
// Created by Anton on 28/11/2021.
//
#include "Player.h"

Player::Player(Type type, const ResourceHolder<sf::Texture, Textures::ID> & textureHolder) :
ActiveEntity(textureHolder.get(toTextureID(type))),
playerType(type)
{
	sf::FloatRect localBounds = Entity::sprite.getLocalBounds();
	this->setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);
}

void Player::LookAt(sf::Vector2f worldPosition)
{
	sf::Vector2f playerPosition = this->getPosition();
	sf::Vector2f facing(worldPosition.x - playerPosition.x, worldPosition.y - playerPosition.y);

	float angle = -atan2f(facing.x, facing.y) * 180.f / M_PI;
	this->setRotation(angle);
}

float Player::getSpeed() const
{
	return speed;
}

unsigned int Player::getCategory() const
{
	return Category::Player;
}