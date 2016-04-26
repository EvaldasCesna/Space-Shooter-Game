#ifndef MENU_MENUSTATE_H
#define MENU_MENUSTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State {
public:
	MenuState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
};
#endif // !MENU_MENUSTATE_H