#include <Menu/UpgradeState.h>
#include <Menu/Button.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

UpgradeState::UpgradeState(StateStack& stack, Context context) :State(stack, context), mGUIContainer() {
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto WeaponButton = std::make_shared<GUI::Button>(context);
	WeaponButton->setPosition(100, 200);
	WeaponButton->setText("Weapon 1");

	auto WeaponButton2 = std::make_shared<GUI::Button>(context);
	WeaponButton2->setPosition(100, 300);
	WeaponButton2->setText("Weapon 2 ");

	auto HullButton = std::make_shared<GUI::Button>(context);
	HullButton->setPosition(100, 400);
	HullButton->setText("Hull");

	auto leftButton = std::make_shared<GUI::Button>(context);
	leftButton->setPosition(400, 500);
	leftButton->setText("<");

	auto RightButton = std::make_shared<GUI::Button>(context);
	RightButton->setPosition(700, 500);
	RightButton->setText(">");

	auto BackButton = std::make_shared<GUI::Button>(context);
	BackButton->setPosition(100, 500);
	BackButton->setText("Back");
	BackButton->setCallback([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(WeaponButton);
	mGUIContainer.pack(WeaponButton2);
	mGUIContainer.pack(HullButton);
	mGUIContainer.pack(leftButton);
	mGUIContainer.pack(RightButton);
	mGUIContainer.pack(BackButton);
}


void UpgradeState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool UpgradeState::update(sf::Time) {
	return true;
}

bool UpgradeState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}