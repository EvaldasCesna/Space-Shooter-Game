#include <Game/CommandQueue.h>
#include <Game/SceneNode.h>

void CommandQueue::push(const Command& command) {
	mQueue.push(command);
}

Command CommandQueue::pop() {
	//Command command = mQueue.back(); if using vector right now missles break but better performance
	//mQueue.pop_back();
	Command command = mQueue.front();
	mQueue.pop();
	return command;
}

bool CommandQueue::isEmpty() const {
	return mQueue.empty();
}
