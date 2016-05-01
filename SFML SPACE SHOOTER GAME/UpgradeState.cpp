#include <Menu/UpgradeState.h>
#include <Menu/Button.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

UpgradeState::UpgradeState(StateStack& stack, Context context) :State(stack, context), mGUIContainer() {
	sf::Vector2f windowSize(context.window->getSize());

	w1 = getContext().player->getWeapon1();
	w2 = getContext().player->getWeapon2();
	h = getContext().player->getHull();
	sh = getContext().player->getShip();


	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	sf::Texture& ShipTexture = context.textures->get(Textures::Player);
	Ship.setTexture(ShipTexture);
	sf::IntRect shipRect(0, 0, 53, 52);
	Ship.setTextureRect(shipRect);
	Ship.setPosition(0.5f * windowSize.x, 0.3f * windowSize.y);
	Ship.setScale(Ship.getScale().x + 5.f, Ship.getScale().y + 5.f);

	sf::Texture& Modifications = context.textures->get(Textures::Modification);
	Weapon1.setTexture(Modifications);
	sf::IntRect weapon1Rect(0, 0, 72, 64);
	Weapon1.setTextureRect(weapon1Rect);
	Weapon1.setPosition(0.3f * windowSize.x, 0.19f * windowSize.y);

	Weapon2.setTexture(Modifications);
	sf::IntRect weapon2Rect(0, 63, 72, 63);
	Weapon2.setTextureRect(weapon2Rect);
	Weapon2.setPosition(0.3f * windowSize.x, 0.39f * windowSize.y);

	Hull.setTexture(Modifications);
	sf::IntRect hullRect(0, 129, 72, 66);
	Hull.setTextureRect(hullRect);
	Hull.setPosition(0.3f * windowSize.x, 0.59f * windowSize.y);

	Weapon1Text = std::make_shared<GUI::Label>("", *context.fonts);
	Weapon1Text->setPosition(0.05f * windowSize.x, 0.3f * windowSize.y);
	Weapon1Text->setText("Normal Laser, with pickups increasing spread.");


	Weapon2Text = std::make_shared<GUI::Label>("", *context.fonts);
	Weapon2Text->setPosition(0.05f * windowSize.x, 0.5f * windowSize.y);
	Weapon2Text->setText("Seeker Missile.");


	HullText = std::make_shared<GUI::Label>("", *context.fonts);
	HullText->setPosition(0.05f * windowSize.x, 0.7f * windowSize.y);
	HullText->setText("Normal Hull.");


	auto WeaponButton = std::make_shared<GUI::Button>(context);
	WeaponButton->setPosition(0.1f * windowSize.x, 0.2f * windowSize.y);
	WeaponButton->setText("Weapon 1");
	WeaponButton->setCallback([this]() {
		w1++;
		updateWeapon1();
	});


	auto WeaponButton2 = std::make_shared<GUI::Button>(context);
	WeaponButton2->setPosition(0.1f * windowSize.x, 0.4f * windowSize.y);
	WeaponButton2->setText("Weapon 2");
	WeaponButton2->setCallback([this]() {
		w2++;
		updateWeapon2();
	});


	auto HullButton = std::make_shared<GUI::Button>(context);
	HullButton->setPosition(0.1f * windowSize.x, 0.6f * windowSize.y);
	HullButton->setText("Hull");
	HullButton->setCallback([this]() {
		h++;
		updateHull();
	});

	auto leftButton = std::make_shared<GUI::Button>(context);
	leftButton->setPosition(0.4f * windowSize.x, 0.8f * windowSize.y);
	leftButton->setText("<");
	leftButton->setCallback([this]() {
		sh--;
		updateShip();
	});


	auto RightButton = std::make_shared<GUI::Button>(context);
	RightButton->setPosition(0.7f * windowSize.x, 0.8f * windowSize.y);
	RightButton->setText(">");
	RightButton->setCallback([this]() {
		sh++;
		updateShip();
	});


	auto BackButton = std::make_shared<GUI::Button>(context);
	BackButton->setPosition(0.1f * windowSize.x, 0.8f * windowSize.y);
	BackButton->setText("Back");
	BackButton->setCallback([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(WeaponButton);
	mGUIContainer.pack(WeaponButton2);
	mGUIContainer.pack(HullButton);
	mGUIContainer.pack(BackButton);
	mGUIContainer.pack(leftButton);
	mGUIContainer.pack(RightButton);
	mGUIContainer.pack(Weapon1Text);
	mGUIContainer.pack(Weapon2Text);
	mGUIContainer.pack(HullText);
}


void UpgradeState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(Weapon1);
	window.draw(Weapon2);
	window.draw(Hull);
	window.draw(Ship);
	window.draw(mGUIContainer);
}

bool UpgradeState::update(sf::Time) {
	updateWeapon1();
	updateWeapon2();
	updateHull();
	updateShip();
	return true;
}

bool UpgradeState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}

void UpgradeState::updateWeapon1() {
	if (w1 > 2) {
		w1 = 1;
	}
	if (w1 == 1) {
		sf::IntRect weapon1Rect(0, 0, 71, 63);
		Weapon1.setTextureRect(weapon1Rect);
		getContext().player->setWeapon1(w1);
		Weapon1Text->setText("Normal Laser, Normal Pickup Upgrades.");
	}
	if (w1 == 2) {
		sf::IntRect weapon1Rect(76, 0, 67, 63);
		Weapon1.setTextureRect(weapon1Rect);
		getContext().player->setWeapon1(w1);
		Weapon1Text->setText("ZigZag Laser, Less Accuracy, More Damage, Lesser Pickups.");
	}
}

void UpgradeState::updateWeapon2() {
	if (w2 > 2) {
		w2 = 1;
	}
	if (w2 == 1) {
		sf::IntRect weapon2Rect(0, 63, 72, 63);
		Weapon2.setTextureRect(weapon2Rect);
		getContext().player->setWeapon2(w2);
		Weapon2Text->setText("Seeker Missile.");

	}
	if (w2 == 2) {
		sf::IntRect weapon2Rect(74, 63, 72, 63);
		Weapon2.setTextureRect(weapon2Rect);
		getContext().player->setWeapon2(w2);
		Weapon2Text->setText("Big Laser.");
	}
}

void UpgradeState::updateHull() {
	if (h > 2) {
		h = 1;
	}
	if (h == 1) {
		sf::IntRect hullRect(0, 129, 72, 66);
		Hull.setTextureRect(hullRect);
		getContext().player->setHull(h);
		HullText->setText("Normal Hull, normal health.");
	}
	if (h == 2) {
		sf::IntRect hullRect(71, 128, 72, 66);
		Hull.setTextureRect(hullRect);
		getContext().player->setHull(h);
		HullText->setText("Rock Hull, less impact damage, less health.");
	}
}

void UpgradeState::updateShip() {
	if (sh < 1) {
		sh = 1;
	}
	if (sh > 3) {
		sh = 3;
	}
	if (sh == 1) {
		Ship.setColor(sf::Color::White);
		getContext().player->setShip(sh);
	}
	if (sh == 2) {
		Ship.setColor(setHue(0, 0, 255));
		getContext().player->setShip(sh);
	}
	if (sh == 3) {
		Ship.setColor(setHue(0, 255, 0));
		getContext().player->setShip(sh);
	}
}