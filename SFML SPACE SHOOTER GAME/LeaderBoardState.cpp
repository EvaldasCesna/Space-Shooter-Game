#include <Menu/Button.h>
#include <Game/Utility.h>
#include <Sound/MusicPlayer.h>
#include <Game/ResourceHolder.h>
#include <Menu\LeaderBoardState.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LeaderBoardState::LeaderBoardState(StateStack& stack, Context context) : State(stack, context), mGUIContainer() {
	sf::Vector2f windowSize(context.window->getSize());
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);


	auto NameLable = std::make_shared<GUI::Label>("PLAYER || ", *getContext().fonts);
	NameLable->setPosition(0.2f * windowSize.x, 0.2f * windowSize.y);
	NameLable->setScale(NameLable->getScale().x + 1.f, NameLable->getScale().y + 1.f);

	auto LevelLable = std::make_shared<GUI::Label>("LEVEL || ", *getContext().fonts);
	LevelLable->setPosition(0.5f * windowSize.x, 0.2f * windowSize.y);
	LevelLable->setScale(LevelLable->getScale().x + 1.f, LevelLable->getScale().y + 1.f);

	auto ScoreLable = std::make_shared<GUI::Label>("SCORE || ", *getContext().fonts);
	ScoreLable->setPosition(0.7f * windowSize.x, 0.2f * windowSize.y);
	ScoreLable->setScale(ScoreLable->getScale().x + 1.f, ScoreLable->getScale().y+ 1.f);

	Scores = std::make_shared<GUI::Label>("", *context.fonts);
	Scores->setPosition(0.2f * windowSize.x, 0.25f * windowSize.y);
	Scores->setScale(ScoreLable->getScale().x + 1.5f, ScoreLable->getScale().y - 0.3f);
	Scores->setText(select());

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.4f * windowSize.x, 0.8f * windowSize.y);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&LeaderBoardState::requestStackPop, this));

	mGUIContainer.pack(NameLable);
	mGUIContainer.pack(LevelLable);
	mGUIContainer.pack(ScoreLable);
	mGUIContainer.pack(backButton);
	mGUIContainer.pack(Scores);
}

void LeaderBoardState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool LeaderBoardState::update(sf::Time) {
	UpdateLeaderboard();
	return true;
}


bool LeaderBoardState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}

void LeaderBoardState::UpdateLeaderboard() {
	Scores->setText(select());
}