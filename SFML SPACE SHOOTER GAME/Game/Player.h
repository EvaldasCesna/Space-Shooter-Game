#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H
#include <Game/Command.h>
#include <SFML/Window/Event.hpp>
#include <map>
class CommandQueue;

class Player {
public:
	enum Action {
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		ActionCount
	};

	enum LevelStatus {
		LevelRunning,
		LevelSuccess,
		LevelFailure
	};
	Player();

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

	void setMissionStatus(LevelStatus status);
	LevelStatus getMissionStatus() const;

	int getScore();
	void resetScore();
	void updateScore(unsigned int in);

private:
	void initializeActions();
	static bool isRealtimeAction(Action action);

	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;
	LevelStatus mCurrentMissionStatus;

	int score;
};
#endif // !GAME_PLAYER_H