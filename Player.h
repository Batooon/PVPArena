//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_PLAYER_H
#define PVPARENA_PLAYER_H

#include "SFML/System.hpp"
#include "CommandQueue.h"

class Player
{
public:
	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleInput(CommandQueue& commands);
};


#endif //PVPARENA_PLAYER_H
