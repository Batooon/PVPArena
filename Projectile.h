//
// Created by Anton on 18/12/2021.
//

#ifndef PVPARENA_PROJECTILE_H
#define PVPARENA_PROJECTILE_H

#include "Entity.h"
#include "Data.h"

class Projectile : public Entity
{
public:
	enum Type
	{
		Bullet,
		EnemyBullet,
		Count
	};

	Projectile(Type type, const TextureHolder& textureHolder);
	unsigned int getCategory() const override;
	sf::FloatRect getBounds() const override;
	float getSpeed() const;
	int getDamage() const;

private:
	void updateCurrent(sf::Time deltaTime, CommandQueue& commands) override;
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	Type type;
	sf::Sprite sprite;
};


#endif //PVPARENA_PROJECTILE_H
