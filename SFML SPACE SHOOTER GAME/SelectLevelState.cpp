#include <Menu/Button.h>
#include <Menu/SelectLevelState.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


SelectLevelState::SelectLevelState(StateStack& stack, Context context) : State(stack, context), mGUIContainer() {
	sf::Vector2f windowSize(context.window->getSize());
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	// If this button is clicked set the level to 1
	auto LevelButton1 = std::make_shared<GUI::Button>(context);
	LevelButton1->setPosition(0.1f * windowSize.x, 0.45f * windowSize.y);
	LevelButton1->setText("1");
	LevelButton1->setCallback([this]() {
		getContext().player->setLevel(1);
		updateLabels();
	
	});

	auto LevelButton2 = std::make_shared<GUI::Button>(context);
	LevelButton2->setPosition(0.3f * windowSize.x, 0.45f * windowSize.y);
	LevelButton2->setText("2");
	LevelButton2->setCallback([this]() {
		getContext().player->setLevel(2);
		updateLabels();

	});

	auto LevelButton3 = std::make_shared<GUI::Button>(context);
	LevelButton3->setPosition(0.5f * windowSize.x, 0.45f * windowSize.y);
	LevelButton3->setText("3");
	LevelButton3->setCallback([this]() {
		getContext().player->setLevel(3);
		updateLabels();
	});

	auto LevelButton4 = std::make_shared<GUI::Button>(context);
	LevelButton4->setPosition(0.7f * windowSize.x, 0.45f * windowSize.y);
	LevelButton4->setText("4");
	LevelButton4->setCallback([this]() {
		getContext().player->setLevel(4);
		updateLabels();
	});

	auto BackButton = std::make_shared<GUI::Button>(context);
	BackButton->setPosition(0.4f * windowSize.x, 0.8f * windowSize.y);
	BackButton->setText("Back");
	BackButton->setCallback([this]() {
		requestStackPop();
	});

    LevelSelct = std::make_shared<GUI::Label>("",*context.fonts);
	LevelSelct->setPosition(0.36f * windowSize.x, 0.6f * windowSize.y);
	LevelSelct->setText("Selected Level 1");
	LevelSelct->setScale(LevelSelct->getScale().x + 1.f, LevelSelct->getScale().y + 1.f);

	mGUIContainer.pack(LevelButton1);
	mGUIContainer.pack(LevelButton2);
	mGUIContainer.pack(LevelButton3);
	mGUIContainer.pack(LevelButton4);
	mGUIContainer.pack(BackButton);
	mGUIContainer.pack(LevelSelct);
}
void SelectLevelState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SelectLevelState::update(sf::Time) {
	updateLabels();
	return true;
}

void SelectLevelState::updateLabels() {
	Player& player = *getContext().player;

	LevelSelct->setText("Selected Level " + std::to_string(player.getLevel()));

}

bool SelectLevelState::handleEvent(const sf::Event& event) {

	mGUIContainer.handleEvent(event);
	return false;
}

