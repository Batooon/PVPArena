//
// Created by Anton on 28/11/2021.
//
#include "Soldier.h"

std::vector<PlayerData> soldierData = initializePlayerData();

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
: Entity(soldierData[type].Health)
, playerType(type)
, sprite(textureHolder.get(soldierData[type].Texture))
, isFiring(false)
, fireCountdown(sf::Time::Zero)
, fireRate(5)
{
	sf::FloatRect localBounds = sprite.getLocalBounds();
	this->setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);

	fireCommand.category = Category::Scene;
	fireCommand.action = [this, &textureHolder](SceneNode& node, sf::Time deltaTime)
	{
		SpawnBullet(node, textureHolder);
	};
}

void Soldier::LookAt(sf::Vector2f worldPosition)
{
	sf::Vector2f playerPosition = this->getPosition();
	sf::Vector2f facing(worldPosition.x - playerPosition.x, worldPosition.y - playerPosition.y);

	float angle = -atan2f(facing.x, facing.y) * 180.f / M_PI;
	this->setRotation(angle);
	facingNormalized = Normalize(facing);
}

unsigned int Soldier::getCategory() const
{
	return Category::Player;
}

float Soldier::getSpeed()
{
	return soldierData[playerType].Speed;
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
	checkFire(deltaTime, commands);

	Entity::updateCurrent(deltaTime, commands);
}

sf::FloatRect Soldier::getBounds() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Soldier::Fire()
{
	isFiring = true;
}

void Soldier::checkFire(sf::Time deltaTime, CommandQueue& commands)
{
	if(isFiring && fireCountdown <= sf::Time::Zero)
	{
		commands.push(fireCommand);
		fireCountdown += sf::seconds(1.f / ((float)fireRate + 1.f));
	}
	else if(fireCountdown > sf::Time::Zero)
	{
		fireCountdown -= deltaTime;
	}
	isFiring = false;
}

void Soldier::CreateProjectile(SceneNode& node, Projectile::Type type, float offsetx, float offsety, const
TextureHolder& textureHolder) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textureHolder));

	sf::Vector2f offset(offsetx * sprite.getGlobalBounds().width + 30 * facingNormalized.x,
	                    offsety * sprite.getGlobalBounds().height + 30 * facingNormalized.y);
	sf::Vector2f velocity(facingNormalized * projectile->getSpeed());

	sf::Vector2f position = getPosition() + offset;
	projectile->setPosition(position + facingNormalized);
	projectile->SetVelocity(velocity);
	node.attachChild(std::move(projectile));
}

void Soldier::SpawnBullet(SceneNode &node, const TextureHolder &textureHolder) const
{
	Projectile::Type type = Projectile::Bullet;
	CreateProjectile(node, type, 0.f, 0.f, textureHolder);
}

sf::Vector2f Soldier::getFacingDirection() const
{
	return facingNormalized;
}
