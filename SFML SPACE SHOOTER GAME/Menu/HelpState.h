#ifndef MENU_HELPSTATE_H
#define MENU_HELPSTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <Menu/Button.h>
#include <Menu/Label.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <array>

class HelpState : public State {
public:
	HelpState(StateStack&, Context);
	virtual void draw();

private:
	void updateLabels();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;

};
#endif // !MENU_HELPSTATE_H