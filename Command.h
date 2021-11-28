//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_COMMAND_H
#define PVPARENA_COMMAND_H

struct Command
{
public:
	Command();
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

#endif //PVPARENA_COMMAND_H
