#include <Menu/HelpState.h>
#include <Game/Utility.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderWindow.hpp>

HelpState::HelpState(StateStack& stack, Context context) : State(stack, context), mGUIContainer() {
	mBackgroundSprite.setTexture(context.textures->get(Textures::Help));
	sf::Vector2f windowSize(context.window->getSize());

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.4f * windowSize.x, 0.8f * windowSize.y);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&HelpState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void HelpState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool HelpState::update(sf::Time) {
	return true;
}

bool HelpState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}

