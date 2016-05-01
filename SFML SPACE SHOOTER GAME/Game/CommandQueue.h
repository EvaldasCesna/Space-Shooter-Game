#ifndef GAME_COMMANDQUEUE_H
#define GAME_COMMANDQUEUE_H
#include <Game/Command.h>
#include <queue>
#include <vector>

class CommandQueue {
public:
	void push(const Command&);
	Command pop();
	bool isEmpty() const;
private:
	std::vector<Command> mVector;
};


#endif // !GAME_COMMANDQUEUE_H