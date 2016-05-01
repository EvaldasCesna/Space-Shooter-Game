#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H
#include <Game/ResourceHolder.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/Player.h>
#include <Game/StateStack.h>
#include <Sound/MusicPlayer.h>
#include <Sound/SoundPlayer.h>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application {
public:
	Application();
	void run();

private:
	void processInput();
	void update(sf::Time);
	void render();

	void updateStatistics(sf::Time);
	void registerStates();

	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	Player mPlayer;

	MusicPlayer mMusic;
	SoundPlayer mSounds;
	StateStack mStateStack;

	sf::Text mStatsText;
	sf::Time mStatsUpdateTime;
	std::size_t mStatsNumFrames;
};

#endif // !GAME_APPLICATION_H