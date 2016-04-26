#ifndef GAME_TITLESTATE_H
#define GAME_TITLESTATE_H
#include <Game/State.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State {
public:
	TitleState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mText;
	bool mShowText;
	sf::Time mTextEffectTime;
};
#endif // !GAME_TITLESTATE_H