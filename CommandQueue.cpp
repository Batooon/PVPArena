//
// Created by Anton on 28/11/2021.
//

#include "CommandQueue.h"

void CommandQueue::push(const Command &command)
{
	queue.push(command);
}

Command CommandQueue::pop()
{
	Command command = queue.front();
	queue.pop();
	return command;
}

bool CommandQueue::isEmpty() const
{
	return queue.empty();
}
