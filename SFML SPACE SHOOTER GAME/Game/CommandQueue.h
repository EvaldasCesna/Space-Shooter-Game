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
	std::queue<Command> mQueue;

	//std::vector<Command> mQueue;

};
#endif // !GAME_COMMANDQUEUE_H