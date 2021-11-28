//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_COMMANDQUEUE_H
#define PVPARENA_COMMANDQUEUE_H

#include "Command.h"
#include <queue>

class CommandQueue
{
public:
	void push(const Command& command);
	Command pop();
	bool isEmpty() const;
private:
	std::queue<Command> queue;
};


#endif //PVPARENA_COMMANDQUEUE_H
