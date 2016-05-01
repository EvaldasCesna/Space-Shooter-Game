#ifndef MENU_UPGRADESTATE_H
#define MENU_UPGRADESTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <Menu/Button.h>
#include <Game/Player.h>
#include <Menu/Label.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class UpgradeState : public State {
public:
	UpgradeState(StateStack&, Context);
	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event&);

	void updateWeapon1();
	void updateWeapon2();
	void updateHull();
	void updateShip();


private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;

	sf::Sprite Weapon1;
	sf::Sprite Weapon2;
	sf::Sprite Hull;
	sf::Sprite Ship;

	GUI::Label::Ptr Weapon1Text;
	GUI::Label::Ptr Weapon2Text;
	GUI::Label::Ptr HullText;

	int sh = 1;
	int h = 1;
	int w1 = 1;
	int w2 = 1;
};
#endif // !MENU_UPGRADESTATE_H