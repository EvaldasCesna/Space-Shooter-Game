#ifndef MENU_UPGRADESTATE_H
#define MENU_UPGRADESTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <Menu/Button.h>
#include <Menu/Label.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class UpgradeState : public State {
public:
	UpgradeState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;

};
#endif // !MENU_UPGRADESTATE_H