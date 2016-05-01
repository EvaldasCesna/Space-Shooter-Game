#include <Menu/GameOverState.h>
#include <Game/Utility.h>
#include <Game/Player.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <Sound/MusicPlayer.h>
#include <Database/Database.h>

GameOverState::GameOverState(StateStack& stack, Context context) : State(stack, context), mGameOverText(), mScoreText(), mNameText(), mElapsedTime(sf::Time::Zero) {
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());
	context.music->play(Music::MenuMusic);
	sf::Event Event;
	score = 0;
	level = 1;
	name = "";

	mGameOverText.setFont(font);
	mScoreText.setFont(font);
	mNameText.setFont(font);
	if (context.player->getMissionStatus() == Player::LevelFailure) {
		mGameOverText.setString("Level Failed!");
		mScoreText.setString("Score: " + std::to_string(context.player->getScore()));
	}
	else {
		mGameOverText.setString("Level Successful!");
		mScoreText.setString("Score: " + std::to_string(context.player->getScore()));
	}


	mNameText.setCharacterSize(70);
	centerOrigin(mNameText);
	mNameText.setPosition(0.1f * windowSize.x, 0.8f * windowSize.y);
	mNameText.setString("Enter Name & press Enter");

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	mScoreText.setCharacterSize(70);
	centerOrigin(mScoreText);
	mScoreText.setPosition(0.5f * windowSize.x, 0.6f * windowSize.y);

	setLevel(context);
	setScore(context);
}

void GameOverState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
	window.draw(mScoreText);
	window.draw(mNameText);
}

bool GameOverState::update(sf::Time dt) {
	mNameText.getString();

	if (n == true) {
		if (name.size() > 0)  name.resize(name.size() - 1);
		insertToDatabase(name, level, score);
		requestStateClear();
		requestStackPush(States::Menu);
	}

	return false;
}

bool GameOverState::handleEvent(const sf::Event& event) {

	if (event.type == sf::Event::TextEntered) {
		if (event.text.unicode < 128) {
			name += (char)event.text.unicode;
		}
	}
	if (event.text.unicode == sf::Keyboard::Return) {
		n = true;
	}
	mNameText.setString(name);

	return false;
}

void GameOverState::insertToDatabase(std::string n, std::string s, int l) {
	insert(n, s, l);
}

void GameOverState::setLevel(Context context) {
	level = std::to_string(context.player->getLevel());

}

void  GameOverState::setScore(Context context) {
	score = context.player->getScore();
}