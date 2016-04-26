#include <Game/GameState.h>
#include <Sound/MusicPlayer.h>
#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack& stack, Context context): State(stack, context), mWorld(*context.window, *context.fonts, *context.sounds), mPlayer(*context.player) {
	mPlayer.setMissionStatus(Player::LevelRunning);
	// Play music
	context.music->play(Music::MissionTheme);
}

void GameState::draw() {
	mWorld.draw();
}

bool GameState::update(sf::Time dt) {
	mWorld.update(dt);

	if (!mWorld.hasAlivePlayer()) {
		mPlayer.setMissionStatus(Player::LevelFailure);
		requestStackPush(States::GameOver);
	}
	//else if (mWorld.hasPlayerReachedEnd())
	else if (mWorld.hasActiveEnemy()) {
		mPlayer.setMissionStatus(Player::LevelSuccess);           //Where new mission can happen
		requestStackPush(States::GameOver);
	}
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event) {
	// Input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);

	// Pause screen trigger
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}