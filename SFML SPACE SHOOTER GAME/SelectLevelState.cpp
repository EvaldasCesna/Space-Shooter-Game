#include <Menu/Button.h>
#include <Menu/SelectLevelState.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <Game/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SelectLevelState::SelectLevelState(StateStack& stack, Context context) : State(stack, context), mGUIContainer() {

	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto LevelButton1 = std::make_shared<GUI::Button>(context);
	LevelButton1->setPosition(90, 350);
	LevelButton1->setText("1");
	LevelButton1->setCallback([this](){

	});

	auto LevelButton2 = std::make_shared<GUI::Button>(context);
	LevelButton2->setPosition(300, 350);
	LevelButton2->setText("2");

	auto LevelButton3 = std::make_shared<GUI::Button>(context);
	LevelButton3->setPosition(510, 350);
	LevelButton3->setText("3");

	auto LevelButton4 = std::make_shared<GUI::Button>(context);
	LevelButton4->setPosition(720, 350);
	LevelButton4->setText("4");

	auto ShipButton = std::make_shared<GUI::Button>(context);
	ShipButton->setPosition(200, 600);
	ShipButton->setText("Ship");

	auto BackButton = std::make_shared<GUI::Button>(context);
	BackButton->setPosition(600, 600);
	BackButton->setText("Back");
	BackButton->setCallback([this]() {
		requestStackPop();
	});

	mGUIContainer.pack(LevelButton1);
	mGUIContainer.pack(LevelButton2);
	mGUIContainer.pack(LevelButton3);
	mGUIContainer.pack(LevelButton4);
	mGUIContainer.pack(ShipButton);
	mGUIContainer.pack(BackButton);
}
void SelectLevelState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SelectLevelState::update(sf::Time) {
	return true;
}

bool SelectLevelState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}