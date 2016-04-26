#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H
#include <Game/State.h>
#include <Game/World.h>
#include <Game/Player.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State {
public:
	GameState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

private:
	World mWorld;
	Player& mPlayer;
};
#endif // !GAME_GAMESTATE_H