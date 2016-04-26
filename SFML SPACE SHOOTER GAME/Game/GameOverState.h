#ifndef GAME_GAMEOVERSTATE_H
#define GAME_GAMEOVERSTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State {
public:
	GameOverState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

private:
	sf::Text mGameOverText;
	sf::Time mElapsedTime;

};
#endif // !GAME_GAMEOVERSTATE_H
