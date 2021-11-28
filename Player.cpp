//
// Created by Anton on 28/11/2021.
//

#include "Player.h"

struct PlayerMover
{
	PlayerMover(float x, float y):velocity(x, y)
	{
	}

	void operator()(SceneNode& node, sf::Time) const
	{
		Soldier& soldier = static_cast<Soldier &>(node);
		soldier.setVelocity(soldier.getVelocity() + velocity);
	}

	sf::Vector2f velocity;
};