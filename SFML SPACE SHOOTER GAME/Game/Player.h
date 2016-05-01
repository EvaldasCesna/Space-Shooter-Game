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
		Special,
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
	void updateScore(int);

	void setLevel(int);
	int getLevel();

	void setHull(int);
	int getHull();

	void setShip(int s);
	int getShip();

	void setWeapon1(int w);
	int getWeapon1();

	void setWeapon2(int w);
	int getWeapon2();

private:
	void initializeActions();
	static bool isRealtimeAction(Action action);

	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;
	LevelStatus mCurrentMissionStatus;

	int mLevel = 1;
	int mScore;
	int mHull = 1;
	int mShip = 1;
	int mWeapon1 = 1;
	int mWeapon2 = 1;
};
#endif // !GAME_PLAYER_H