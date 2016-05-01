#ifndef GAME_WORLD_H
#define GAME_WORLD_H
#include <Game/ResourceHolder.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/SceneNode.h>
#include <Game/SpriteNode.h>
#include <Game/Spaceship.h>
#include <Game/CommandQueue.h>
#include <Game/Command.h>
#include <Game/BloomEffect.h>
#include <Sound/SoundPlayer.h>
#include <Game/Entity.h>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <array>
#include <queue>



// Forward declaration
namespace sf {
	class RenderTarget;
}

class World : private sf::NonCopyable {
public:
	World(sf::RenderTarget&, FontHolder&, SoundPlayer&, Player&);
	void update(sf::Time);
	void draw();
	CommandQueue& getCommandQueue();

	bool hasAlivePlayer() const;
	bool hasActiveEnemy() const;
	void setLevel(int lin);

private:
	void loadTextures();
	void adaptPlayerPosition();
	void adaptPlayerVelocity();
	void handleCollisions();
	void updateSounds();

	void buildScene();
	void addEnemies();
	void endless();
	void updateTime(sf::Time & tIn);
	sf::Time getTime();
	void addEnemy(Spaceship::Type, float, float);

	void spawnEnemies();
	void destroyEntitiesOutsideView();
	void guideMissiles();
	sf::FloatRect getViewBounds() const;
	sf::FloatRect getBattlefieldBounds() const;

	enum Layer {
		Background,
		LowerAir,
		UpperAir,
		LayerCount
	};

	struct SpawnPoint {
		SpawnPoint(Spaceship::Type type, float x, float y) : type(type), x(x), y(y) { }
		Spaceship::Type type;
		float x;
		float y;
	};

	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mWorldView;
	sf::View mBackView;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;
	Player& mPlayer;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	CommandQueue mCommandQueue;

	sf::Time mTime;
	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed;
	Spaceship* mPlayerAircraft;
	int mLevel;
	int mScore;
	std::vector<SpawnPoint> mEnemySpawnPoints;
	std::vector<Spaceship*> mActiveEnemies;

	BloomEffect mBloomEffect;

};
#endif // !GAME_WORLD_H