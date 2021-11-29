//
// Created by Anton on 28/11/2021.
//

#include "Player.h"

template<class GameObject, class Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function f)
{
	return [=] (SceneNode& node, sf::Time deltaTime)
	{
		f(static_cast<GameObject&>(node), deltaTime);
	};
}

struct PlayerMover
{
	PlayerMover(float x, float y):velocity(x, y)
	{
	}

	void operator()(Soldier& soldier, sf::Time deltaTime) const
	{
		soldier.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player()
{
	keyBinds[sf::Keyboard::A] = MoveLeft;
	keyBinds[sf::Keyboard::D] = MoveRight;
	keyBinds[sf::Keyboard::W] = MoveUp;
	keyBinds[sf::Keyboard::S] = MoveDown;

	initializeCommands();

	for(auto &pair: actionBinds)
		pair.second.category = Category::Player;
}

void Player::initializeCommands()
{
	actionBinds[MoveLeft].action = derivedAction<Soldier>(PlayerMover(-playerSpeed, 0));
	actionBinds[MoveRight].action = derivedAction<Soldier>(PlayerMover(playerSpeed, 0));
	actionBinds[MoveUp].action = derivedAction<Soldier>(PlayerMover(0, -playerSpeed));
	actionBinds[MoveDown].action = derivedAction<Soldier>(PlayerMover(0, playerSpeed));
}

void Player::handleInput(CommandQueue &commands)
{
	for(auto pair:keyBinds)
	{
		if(sf::Keyboard::isKeyPressed(pair.first))
			commands.push(actionBinds[pair.second]);
	}
}
