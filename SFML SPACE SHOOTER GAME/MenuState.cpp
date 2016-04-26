#include <Menu/MenuState.h>
#include <Menu/Button.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context): State(stack, context), mGUIContainer() {
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(440, 300);
	playButton->setText("Play");
	playButton->setCallback([this]() {
		requestStackPop();
		requestStackPush(States::Play);
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(440, 350);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]() {
		requestStackPush(States::Settings);
	});

	auto helpButton = std::make_shared<GUI::Button>(context);
	helpButton->setPosition(440, 400);
	helpButton->setText("Help");
	helpButton->setCallback([this]() {
		requestStackPush(States::Help);
	});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(440, 600);
	exitButton->setText("Exit");
	exitButton->setCallback([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(helpButton);
	mGUIContainer.pack(exitButton);
	// Menu music
	context.music->play(Music::MenuTheme);
}

void MenuState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time) {
	return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}
