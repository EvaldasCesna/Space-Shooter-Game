#include <Game/CommandQueue.h>
#include <Game/SceneNode.h>

void CommandQueue::push(const Command& command) {
	// Reserve could be used for better performance
	mVector.reserve(1000);
	mVector.push_back(command);
}

Command CommandQueue::pop() {
	Command command = mVector.back(); 
	mVector.pop_back();
	return command;
}

bool CommandQueue::isEmpty() const {
	return mVector.empty();
}
