//
// Created by Anton on 18/12/2021.
//

#include "Projectile.h"

std::vector<ProjectileData> projectileData = initializeProjectileData();

Projectile::Projectile(Projectile::Type type, const TextureHolder &textureHolder)
: Entity(1)
, type(type)
, sprite(textureHolder.get(projectileData[type].Texture))
{
}

unsigned int Projectile::getCategory() const
{
	switch(type)
	{
		case Type::Bullet:
			return Category::Projectile;
		case Type::EnemyBullet:
			return Category::EnemyProjectile;
	}
}

sf::FloatRect Projectile::getBounds() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

float Projectile::getSpeed() const
{
	return projectileData[type].Speed;
}

int Projectile::getDamage() const
{
	return projectileData[type].Damage;
}

void Projectile::updateCurrent(sf::Time deltaTime, CommandQueue &commands)
{
	Entity::updateCurrent(deltaTime, commands);
}

void Projectile::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
