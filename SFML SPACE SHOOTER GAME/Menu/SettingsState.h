#ifndef MENU_SETTINGSSTATE_H
#define MENU_SETTINGSSTATE_H
#include <Game/State.h>
#include <Game/Player.h>
#include <Menu/Container.h>
#include <Menu/Button.h>
#include <Menu/Label.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <array>

class SettingsState : public State {
public:
	SettingsState(StateStack&, Context);

	virtual void draw();
	virtual bool update(sf::Time);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();
	void addButtonLabel(Player::Action, float, const std::string&, Context);

	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, Player::ActionCount> mBindingButtons;
	std::array<GUI::Label::Ptr, Player::ActionCount> mBindingLabels;
};
#endif // !MENU_SETTINGSSTATE_H