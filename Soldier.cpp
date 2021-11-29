//
// Created by Anton on 28/11/2021.
//
#include "Soldier.h"

Textures::ID toTextureID(Soldier::Type type)
{
	switch(type)
	{
		case Soldier::DoubleGun:
			return Textures::DoubleGun;
		case Soldier::LaserGun:
			return Textures::LaserGun;
		case Soldier::MagmaGun:
			return Textures::MagmaGun;
	}
}

Soldier::Soldier(Type type, const ResourceHolder<sf::Texture, Textures::ID> & textureHolder) :
ActiveEntity(textureHolder.get(toTextureID(type))),
playerType(type)
{
	sf::FloatRect localBounds = Entity::sprite.getLocalBounds();
	this->setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);
}

void Soldier::LookAt(sf::Vector2f worldPosition)
{
	sf::Vector2f playerPosition = this->getPosition();
	sf::Vector2f facing(worldPosition.x - playerPosition.x, worldPosition.y - playerPosition.y);

	float angle = -atan2f(facing.x, facing.y) * 180.f / M_PI;
	this->setRotation(angle);
}

unsigned int Soldier::getCategory() const
{
	return Category::Player;
}