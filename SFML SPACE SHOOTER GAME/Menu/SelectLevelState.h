#ifndef MENU_SELECTLEVELSTATE_H
#define MENU_SELECTLEVELSTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <Menu/Button.h>
#include <Menu/Label.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SelectLevelState : public State {
public:
	SelectLevelState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;

};
#endif // !MENU_SELECTLEVELSTATE_H