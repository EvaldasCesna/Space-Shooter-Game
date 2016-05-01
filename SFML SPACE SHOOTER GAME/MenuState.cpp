#include <Menu/MenuState.h>
#include <Menu/Button.h>
#include <Game/Utility.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context): State(stack, context), mGUIContainer() {
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);
	sf::Vector2f windowSize(context.window->getSize());
	

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(0.4f * windowSize.x, 0.4f * windowSize.y);
	playButton->setText("Play");
	playButton->setCallback([this]() {
		requestStackPop();
		requestStackPush(States::Play);
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(0.4f * windowSize.x, 0.5f * windowSize.y);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]() {
		requestStackPush(States::Settings);
	});

	auto helpButton = std::make_shared<GUI::Button>(context);
	helpButton->setPosition(0.4f * windowSize.x, 0.6f * windowSize.y);
	helpButton->setText("Help");
	helpButton->setCallback([this]() {
		requestStackPush(States::Help);
	});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(0.4f * windowSize.x, 0.8f * windowSize.y);
	exitButton->setText("Exit");
	exitButton->setCallback([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(helpButton);
	mGUIContainer.pack(exitButton);

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
