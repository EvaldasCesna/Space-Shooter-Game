#include <Menu/Button.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <Menu\LeaderBoardState.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LeaderBoardState::LeaderBoardState(StateStack& stack, Context context) : State(stack, context), mGUIContainer() {

	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto NameLable = std::make_shared<GUI::Label>("PLAYER || ", *getContext().fonts);
	NameLable->setPosition(250.f, 200.f);
	NameLable->setScale(1, 2);

	auto LevelLable = std::make_shared<GUI::Label>("LEVEL || ", *getContext().fonts);
	LevelLable->setPosition(450.f, 200.f);
	LevelLable->setScale(1, 2);

	auto ScoreLable = std::make_shared<GUI::Label>("SCORE || ", *getContext().fonts);
	ScoreLable->setPosition(650.f, 200.f);
	ScoreLable->setScale(1, 2);

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(400.f, 600.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&LeaderBoardState::requestStackPop, this));

	mGUIContainer.pack(NameLable);
	mGUIContainer.pack(LevelLable);
	mGUIContainer.pack(ScoreLable);
	mGUIContainer.pack(backButton);
}

void LeaderBoardState::draw() {
	sf::RenderWindow& window = *getContext().window;


	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool LeaderBoardState::update(sf::Time) {
	return true;
}


bool LeaderBoardState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}