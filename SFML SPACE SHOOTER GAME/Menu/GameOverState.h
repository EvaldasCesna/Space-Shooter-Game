#ifndef GAME_GAMEOVERSTATE_H
#define GAME_GAMEOVERSTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <String>

class GameOverState : public State {
public:
	GameOverState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

	void insertToDatabase(std::string n, std::string s, int l);

	void setLevel(Context);

	void setScore(Context);

	//void insertToDatabase(std::string n, std::string l, std::string s);
	sf::Event Event;

private:
	bool n = false;
	std::string name;
	sf::Text mGameOverText;
	sf::Time mElapsedTime;
	sf::Text mScoreText;
	sf::Text mNameText;
	int score;
	std::string level;
};
#endif // !GAME_GAMEOVERSTATE_H
