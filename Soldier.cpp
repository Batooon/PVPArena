//
// Created by Anton on 28/11/2021.
//
#include "Soldier.h"

std::vector<PlayerData> Data = initializePlayerData();

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

Soldier::Soldier(Type type, const ResourceHolder<sf::Texture, Textures::ID> & textureHolder)
: Entity(Data[type].Health)
, playerType(type)
, sprite(textureHolder.get(Data[type].Texture))
{
	sf::FloatRect localBounds = sprite.getLocalBounds();
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

float Soldier::getSpeed()
{
	return Data[playerType].Speed;
}

void Soldier::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Soldier::updateCurrent(sf::Time deltaTime, CommandQueue &commands)
{
	if(isDead())
	{
		//Destroy object from scene
		return;
	}
	Entity::updateCurrent(deltaTime, commands);
}
