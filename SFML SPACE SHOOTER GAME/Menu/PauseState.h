#ifndef MENU_PAUSESTATE_H
#define MENU_PAUSESTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State {
public:
	PauseState(StateStack&, Context);
	~PauseState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mPausedText;
	GUI::Container mGUIContainer;
};
#endif // !MENU_PAUSESTATE_H
