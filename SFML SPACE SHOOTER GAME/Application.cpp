#include <Game/Application.h>
#include <Game/Utility.h>
#include <Game/State.h>
#include <Game/StateIdentifiers.h>
#include <Menu/TitleState.h>
#include <Game/GameState.h>
#include <Menu/SettingsState.h>
#include <Menu/GameOverState.h>
#include <Menu/MenuState.h>
#include <Menu/PauseState.h>
#include <Menu/HelpState.h>
#include <Menu/PlayState.h>
#include <Menu/SelectLevelState.h>
#include <Menu/UpgradeState.h>
#include <Menu/LeaderBoardState.h>

/***************************************************************************************
    *    Title:SFML Game Development
    *    Author:Haller J,Hansson H.V, Moreira A
    *    Date:June 2013
    *    Availability: https://www.packtpub.com/game-development/sfml-game-development
    *
    ************************************************************************************/
	//Used the book for grounds of our game

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application() : mWindow(sf::VideoMode(1024, 768), "SFML Space Shooter Game", sf::Style::Close)
, mTextures(), mFonts(), mPlayer(), mMusic(), mSounds()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
, mStatsText(), mStatsUpdateTime(), mStatsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	//Menu Textures and font are loaded here 
	mFonts.load(Fonts::Main, "Media/Consolas Bold.ttf");
	mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	mTextures.load(Textures::Help, "Media/Textures/help.png");
	mTextures.load(Textures::Buttons, "Media/Textures/Buttons.png");
	mTextures.load(Textures::Modification, "Media/Textures/Modification.png");
	mTextures.load(Textures::Player, "Media/Textures/Player.png");

	mStatsText.setFont(mFonts.get(Fonts::Main));
	mStatsText.setPosition(5.f, 5.f);
	mStatsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);
	mMusic.setVolume(15.f);
}

void Application::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()) {
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);
			// Checks if the stack is empty before it updates anything to prevent unecessary updates
			if (mStateStack.isEmpty())
				mWindow.close();
		}
		updateStatistics(dt);
		render();
	}
}

void Application::processInput() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt) {
	mStateStack.update(dt);
}

void Application::render() {
	mWindow.clear();
	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatsText);
	mWindow.display();
}

void Application::updateStatistics(sf::Time dt) {
	mStatsUpdateTime += dt;
	mStatsNumFrames += 1;
	if (mStatsUpdateTime >= sf::seconds(1.0f)) {
		mStatsText.setString("FPS: " + std::to_string(mStatsNumFrames));
		mStatsUpdateTime -= sf::seconds(1.0f);
		mStatsNumFrames = 0;
	}
}

void Application::registerStates() {
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<PlayState>(States::Play);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<UpgradeState>(States::Upgrade);
	mStateStack.registerState<SelectLevelState>(States::LevelSelect);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<HelpState>(States::Help);
	mStateStack.registerState<LeaderBoardState>(States::Leader);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
}
