//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_PLAYER_H
#define PVPARENA_PLAYER_H

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "CommandQueue.h"
#include "Soldier.h"
#include <map>

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		Count
	};

	Player();
	void handleInput(CommandQueue& commands);

private:
	void initializeCommands();
	std::map<sf::Keyboard::Key, Action> keyBinds;
	std::map<Action, Command> actionBinds;
};


#endif //PVPARENA_PLAYER_H
