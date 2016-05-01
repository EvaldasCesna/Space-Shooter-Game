#include <Game/World.h>
#include <Game/Projectile.h>
#include <Game/Pickup.h>
#include <Game/TextNode.h>
#include <Game/ParticleNode.h>
#include <Sound/SoundNode.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdlib.h> 
#include <Game/Utility.h>

#include <iostream>


World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, Player& player) : mTarget(outputTarget)
, mSceneTexture()
, mWorldView(outputTarget.getDefaultView())
, mTextures()
, mFonts(fonts)
, mSounds(sounds)
, mPlayer(player)
, mSceneGraph()
, mSceneLayers()
, mLevel(player.getLevel())
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 50000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
, mEnemySpawnPoints()
, mActiveEnemies()
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
	loadTextures();
	buildScene();
	mPlayer.resetScore();

	// Set the begging state of the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt) {
	

	// Scrolls the view
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	//Player default velocity
	mPlayerAircraft->setVelocity(0.f, 0.f);
	// Command to destroy unused entities
	destroyEntitiesOutsideView();
	//Missile guider
	guideMissiles();

	// Forwards the commands to the scene graph, adapts velocity/scrolling/diagonal correction
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	// Collisions
	handleCollisions();

	// Remove the destroyed entities
	mSceneGraph.removeWrecks();
	spawnEnemies();

	// Update the scene and make sure the view is on player
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();

	// Update the sound playing
	updateSounds();

	// Update score
	mPlayer.updateScore(mPlayerAircraft->getScore());
}

void World::draw() {
	
	if (PostEffect::isSupported()) {
		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);
	}
	else {
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
	}
}

CommandQueue& World::getCommandQueue() {
	return mCommandQueue;
}

bool World::hasAlivePlayer() const {
	return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasActiveEnemy() const {
	if (mActiveEnemies.size() < 1 && mEnemySpawnPoints.size() < 1)
		return true;
	else
		return false;
}

void World::loadTextures() {
	
	mTextures.load(Textures::Player, "Media/Textures/Player.png");
	mTextures.load(Textures::Enemy, "Media/Textures/Enemy.png");
	mTextures.load(Textures::Shots, "Media/Textures/Shots.png");
	mTextures.load(Textures::Entities, "Media/Textures/Entities.png");
	if (mLevel == 1) {
		mTextures.load(Textures::Space, "Media/Textures/Space.png");
		mTextures.load(Textures::Enviroment, "Media/Textures/Rocks.png");
	}
	if (mLevel == 2) {
		mTextures.load(Textures::Space2, "Media/Textures/Space2.png");
	}
	if (mLevel == 3) {
		mTextures.load(Textures::Space3, "Media/Textures/Space3.png");
	}
	if (mLevel == 4) {
		mTextures.load(Textures::Space4, "Media/Textures/Space4.png");
	}
	mTextures.load(Textures::Boss, "Media/Textures/Boss.png");
	mTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");
	mTextures.load(Textures::Particle, "Media/Textures/Particle.png");
}

void World::adaptPlayerPosition() {
	// Players position is kept within the view
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity() {
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	// Keeps the same velocity while going up
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

	// Add extra velocity
	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2) {
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// If a player hits an enemy hit detection happens enemies cant hit eachother
	if (type1 & category1 && type2 & category2) {
		return true;
	}
	else if (type1 & category2 && type2 & category1) {
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else {
		return false;
	}
}

void World::handleCollisions() {
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (SceneNode::Pair pair : collisionPairs) {
		if (matchesCategories(pair, Category::PlayerSpaceship, Category::EnemySpaceship)) {
			auto& player = static_cast<Spaceship&>(*pair.first);
			auto& enemy = static_cast<Spaceship&>(*pair.second);

			// Collision damage depending on hull
			if (mPlayer.getHull() == 1) {
				player.damage(enemy.getHitpoints());
			}
			if (mPlayer.getHull() == 2) {
				player.damage(5);
			}

			// Give score for collision based on enemy hp
			mPlayerAircraft->setScore(enemy.getHitpoints());
			mScore += mPlayerAircraft->getScore();

			if (enemy.getHitpoints() < 500 ) {
				enemy.destroy();
			}
		}
		else if (matchesCategories(pair, Category::PlayerSpaceship, Category::Pickup)) {
			auto& player = static_cast<Spaceship&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);
			// Player can take pickups if collides with one
			pickup.apply(player);
			pickup.destroy();
			player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
		}

		else if (matchesCategories(pair, Category::EnemySpaceship, Category::AlliedProjectile) || matchesCategories(pair, Category::PlayerSpaceship, Category::EnemyProjectile)) {
			auto& aircraft = static_cast<Spaceship&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			// damage calculation and projectile destruction
			aircraft.damage(projectile.getDamage());

			// Score per hit
			if (aircraft.getCategory() == Category::EnemySpaceship) {
				mPlayerAircraft->setScore(projectile.getDamage());
				mScore += mPlayerAircraft->getScore();
			}
			
			// Short sleep on hit
			sf::sleep(sf::milliseconds(5));
			if (projectile.isBigLaser()) { }
			else {
				projectile.destroy();
			}
		}
	}
}

void World::updateSounds() {
	// The listeener follows the player
	mSounds.setListenerPosition(mPlayerAircraft->getWorldPosition());
	// Remove sounds
	mSounds.removeStoppedSounds();
}

void World::buildScene() {
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i) {
		Category::Type category = (i == LowerAir) ? Category::SceneSpaceLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	if (mLevel == 1) {
		sf::Texture& spaceEnTexture = mTextures.get(Textures::Enviroment);
		spaceEnTexture.setRepeated(true);

		sf::Texture& spaceTexture = mTextures.get(Textures::Space);
		spaceTexture.setRepeated(true);

		float viewHeight = mWorldView.getSize().y;
		sf::IntRect textureRect(mWorldBounds);
		textureRect.height += static_cast<int>(viewHeight);

		// Add the background sprite to the scene
		std::unique_ptr<SpriteNode> spaceSprite(new SpriteNode(spaceTexture, textureRect));
		spaceSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
		mSceneLayers[Background]->attachChild(std::move(spaceSprite));

		std::unique_ptr<SpriteNode> spaceEnSprite(new SpriteNode(spaceEnTexture, textureRect));
		spaceEnSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
		mSceneLayers[Background]->attachChild(std::move(spaceEnSprite));
	}
	if (mLevel == 2) {
		sf::Texture& spaceTexture = mTextures.get(Textures::Space2);
		spaceTexture.setRepeated(true);

		float viewHeight = mWorldView.getSize().y;
		sf::IntRect textureRect(mWorldBounds);
		textureRect.height += static_cast<int>(viewHeight);

		// Add the background sprite to the scene
		std::unique_ptr<SpriteNode> spaceSprite(new SpriteNode(spaceTexture, textureRect));
		spaceSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
		mSceneLayers[Background]->attachChild(std::move(spaceSprite));
	}
	if (mLevel == 3) {
		sf::Texture& spaceTexture = mTextures.get(Textures::Space3);
		spaceTexture.setRepeated(true);

		float viewHeight = mWorldView.getSize().y;
		sf::IntRect textureRect(mWorldBounds);
		textureRect.height += static_cast<int>(viewHeight);

		// Add the background sprite to the scene
		std::unique_ptr<SpriteNode> spaceSprite(new SpriteNode(spaceTexture, textureRect));
		spaceSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
		mSceneLayers[Background]->attachChild(std::move(spaceSprite));
	}
	if (mLevel == 4) {
		sf::Texture& spaceTexture = mTextures.get(Textures::Space4);
		spaceTexture.setRepeated(true);

		float viewHeight = mWorldView.getSize().y;
		sf::IntRect textureRect(mWorldBounds);
		textureRect.height += static_cast<int>(viewHeight);

		// Add the background sprite to the scene
		std::unique_ptr<SpriteNode> spaceSprite(new SpriteNode(spaceTexture, textureRect));
		spaceSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
		mSceneLayers[Background]->attachChild(std::move(spaceSprite));
	}

	// Add particle node to the scene
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));

	// Add propellant particle node to the scene
	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

	// Add player's aircraft
	if (mPlayer.getHull() == 1) {
		std::unique_ptr<Spaceship> player(new Spaceship(Spaceship::Player1, mTextures, mFonts, mPlayer));

		mPlayerAircraft = player.get();

		mPlayerAircraft->setPosition(mSpawnPosition);
		mSceneLayers[UpperAir]->attachChild(std::move(player));
	}

	if (mPlayer.getHull() == 2) {
		std::unique_ptr<Spaceship> player(new Spaceship(Spaceship::Player2, mTextures, mFonts, mPlayer));

		mPlayerAircraft = player.get();

		mPlayerAircraft->setPosition(mSpawnPosition);
		mSceneLayers[UpperAir]->attachChild(std::move(player));
	}
	// Add enemy aircraft
	addEnemies();
}

void World::addEnemies() {	  
	// Add enemies to the spawn point container
	if (mLevel == 1) {

		addEnemy(Spaceship::Enemy3, -200.f, 500.f);
		addEnemy(Spaceship::Enemy1, 0.f, 500.f);
		addEnemy(Spaceship::Enemy1, +100.f, 750.f);
		addEnemy(Spaceship::Enemy1, -100.f, 850.f);
		addEnemy(Spaceship::Enemy1, 0.f, 1000.f);
		addEnemy(Spaceship::Enemy1, -100.f, 1000.f);
		addEnemy(Spaceship::Enemy1, -200.f, 1000.f);
		addEnemy(Spaceship::Enemy3, 200.f, 1100.f);
		addEnemy(Spaceship::Enemy1, +100.f, 1150.f);
		addEnemy(Spaceship::Enemy1, -100.f, 1150.f);
		addEnemy(Spaceship::Enemy4, -500.f, 1300.f);
		addEnemy(Spaceship::Enemy5, 500.f, 1300.f);
		addEnemy(Spaceship::Enemy2, 70.f, 1500.f);
		addEnemy(Spaceship::Enemy2, -70.f, 1500.f);
		addEnemy(Spaceship::Enemy2, -70.f, 1710.f);
		addEnemy(Spaceship::Enemy2, 70.f, 1700.f);
		addEnemy(Spaceship::Enemy2, 30.f, 1850.f);
		addEnemy(Spaceship::Enemy1, 300.f, 2200.f);
		addEnemy(Spaceship::Enemy1, -300.f, 2200.f);
		addEnemy(Spaceship::Enemy1, 0.f, 2200.f);
		addEnemy(Spaceship::Enemy1, 0.f, 2500.f);
		addEnemy(Spaceship::Enemy4, -500.f, 2600.f);
		addEnemy(Spaceship::Enemy2, -300.f, 2700.f);
		addEnemy(Spaceship::Enemy2, -300.f, 2700.f);
		addEnemy(Spaceship::Enemy1, 0.f, 3000.f);
		addEnemy(Spaceship::Enemy1, 100.f, 3100.f);
		addEnemy(Spaceship::Enemy1, 250.f, 3250.f);
		addEnemy(Spaceship::Enemy1, -250.f, 3250.f);
		addEnemy(Spaceship::Enemy2, 0.f, 3500.f);
		addEnemy(Spaceship::Enemy2, 0.f, 3700.f);
		addEnemy(Spaceship::Enemy1, 0.f, 3800.f);
		addEnemy(Spaceship::Enemy2, 0.f, 4000.f);
		addEnemy(Spaceship::Enemy2, -200.f, 4200.f);
		addEnemy(Spaceship::Enemy1, 200.f, 4200.f);
		addEnemy(Spaceship::Enemy1, 0.f, 4400.f);
		addEnemy(Spaceship::Enemy3, -400.f, 4400.f);
		addEnemy(Spaceship::Enemy3, -300.f, 4400.f);
		addEnemy(Spaceship::Enemy3, 400.f, 4400.f);
		addEnemy(Spaceship::Enemy3, 300.f, 4400.f);
		addEnemy(Spaceship::Enemy2, 0.f, 4500.f);
	}

	if (mLevel == 2) {
		addEnemy(Spaceship::Enemy3, -300.f, 500.f);
		addEnemy(Spaceship::Enemy3, -200.f, 500.f);
		addEnemy(Spaceship::Enemy3, -100.f, 500.f);
		addEnemy(Spaceship::Enemy3, 0.f, 500.f);
		addEnemy(Spaceship::Enemy3, 100.f, 500.f);
		addEnemy(Spaceship::Enemy3, 200.f, 500.f);
		addEnemy(Spaceship::Enemy3, 300.f, 500.f);
		addEnemy(Spaceship::Enemy1, -400.f, 800.f);
		addEnemy(Spaceship::Enemy1, 400.f, 800.f);
		addEnemy(Spaceship::Enemy2, 0.f, 1000.f);
		addEnemy(Spaceship::Enemy2, -100.f, 1000.f);
		addEnemy(Spaceship::Enemy2, -200.f, 1000.f);
		addEnemy(Spaceship::Enemy4, -500.f, 1200.f);
		addEnemy(Spaceship::Enemy5, 500.f, 1200.f);
		addEnemy(Spaceship::Enemy3, 200.f, 1050.f);
		addEnemy(Spaceship::Enemy1, +100.f, 1150.f);
		addEnemy(Spaceship::Enemy2, 0.f, 1150.f);
		addEnemy(Spaceship::Enemy1, -100.f, 1150.f);
		addEnemy(Spaceship::Enemy2, 70.f, 1300.f);
		addEnemy(Spaceship::Enemy2, -70.f, 1300.f);
		addEnemy(Spaceship::Enemy2, 0.f, 1310.f);
		addEnemy(Spaceship::Enemy3, -300.f, 1500.f);
		addEnemy(Spaceship::Enemy3, 300.f, 1500.f);
		addEnemy(Spaceship::Enemy2, -70.f, 1710.f);
		addEnemy(Spaceship::Enemy1, 70.f, 1710.f);
		addEnemy(Spaceship::Enemy2, -70.f, 1800.f);
		addEnemy(Spaceship::Enemy1, 70.f, 1800.f);
		addEnemy(Spaceship::Enemy2, 200.f, 1800.f);
		addEnemy(Spaceship::Enemy1, -200.f, 1800.f);
		addEnemy(Spaceship::Enemy2, 30.f, 1900.f);
		addEnemy(Spaceship::Enemy1, 200.f, 2200.f);
		addEnemy(Spaceship::Enemy1, -200.f, 2200.f);
		addEnemy(Spaceship::Enemy5, 500.f, 2300.f);
		addEnemy(Spaceship::Enemy2, 0.f, 2500.f);
		addEnemy(Spaceship::Enemy2, 100.f, 2600.f);
		addEnemy(Spaceship::Enemy3, -200.f, 2600.f);
		addEnemy(Spaceship::Enemy2, 200.f, 2700.f);
		addEnemy(Spaceship::Enemy1, -200.f, 3000.f);
		addEnemy(Spaceship::Enemy1, 0.f, 3000.f);
		addEnemy(Spaceship::Enemy1, 100.f, 3100.f);
		addEnemy(Spaceship::Enemy1, 300.f, 3250.f);
		addEnemy(Spaceship::Enemy1, -250.f, 3250.f);
		addEnemy(Spaceship::Enemy2, 0.f, 3400.f);
		addEnemy(Spaceship::Enemy2, 0.f, 3500.f);
		addEnemy(Spaceship::Enemy2, 0.f, 3600.f);
		addEnemy(Spaceship::Enemy2, 0.f, 3700.f);
		addEnemy(Spaceship::Enemy1, 0.f, 3800.f);
		addEnemy(Spaceship::Enemy2, 0.f, 4000.f);
		addEnemy(Spaceship::Enemy2, -200.f, 4200.f);
		addEnemy(Spaceship::Enemy3, 200.f, 4200.f);
		addEnemy(Spaceship::Enemy3, 100.f, 4300.f);
		addEnemy(Spaceship::Enemy3, -100.f, 4300.f);

		addEnemy(Spaceship::Boss, 0.f, 4200.f);

	}
	if (mLevel == 3) {
		addEnemy(Spaceship::Enemy2, -300.f, 500.f);
		addEnemy(Spaceship::Enemy1, -400.f, 700.f);
		addEnemy(Spaceship::Enemy3, 300.f, 1200.f);
		addEnemy(Spaceship::Enemy2, 0.f, 1900.f);
		addEnemy(Spaceship::Enemy3, -300.f, 2400.f);
		addEnemy(Spaceship::Enemy1, 0.f, 1500.f);
		addEnemy(Spaceship::Enemy2, 600.f, 1500.f);
		addEnemy(Spaceship::Enemy1, +350.f, 2500.f);
		addEnemy(Spaceship::Enemy3, -800.f, 3500.f);
		addEnemy(Spaceship::Enemy1, 0.f, 3500.f);
		addEnemy(Spaceship::Enemy3, 800.f, 2000.f);
		addEnemy(Spaceship::Enemy3, 400.f, 1500.f);
		addEnemy(Spaceship::Enemy3, -900.f, 1500.f);
		addEnemy(Spaceship::Enemy3, 300.f, 4000.f);
		addEnemy(Spaceship::Enemy3, -600.f, 5500.f);
		addEnemy(Spaceship::Enemy3, 450.f, 500.f);
		addEnemy(Spaceship::Enemy3, -800.f, 999.f);
		addEnemy(Spaceship::Enemy3, 666.f, 666.f);
		addEnemy(Spaceship::Enemy3, -420.f, 2016.f);
		addEnemy(Spaceship::Enemy3, -300.f, 5500.f);
		addEnemy(Spaceship::Enemy3, 300.f, 1945.f);
		addEnemy(Spaceship::Enemy3, -5.f, 1953.f);
		addEnemy(Spaceship::Enemy3, 100.f, 1000.f);
		addEnemy(Spaceship::Boss, -400.f, 2200.f);
		addEnemy(Spaceship::Boss, +400.f, 2200.f);
		addEnemy(Spaceship::Enemy3, -280.f, 5691.f);
		addEnemy(Spaceship::Enemy3, 780.f, 2970.f);
		addEnemy(Spaceship::Enemy3, -600.f, 5475.f);
		addEnemy(Spaceship::Enemy1, -300.f, 3581.f);
		addEnemy(Spaceship::Enemy2, 229.f, 4890.f);
		addEnemy(Spaceship::Enemy3, 0.f, 5500.f);
		addEnemy(Spaceship::Enemy3, -420.f, 4200.f);
		addEnemy(Spaceship::Enemy2, 420.f, 4200.f);
		addEnemy(Spaceship::Enemy1, -200.f, 5500.f);
		addEnemy(Spaceship::Enemy2, 550.f, 2010.f);
		addEnemy(Spaceship::Enemy1, -100.f, 3000.f);
		addEnemy(Spaceship::Enemy2, 700.f, 2600.f);
		addEnemy(Spaceship::Enemy1, -700.f, 2600.f);
		addEnemy(Spaceship::Enemy2, 0.f, 2600.f);
		addEnemy(Spaceship::Enemy1, -420.f, 5500.f);
		addEnemy(Spaceship::Enemy2, 420.f, 5500.f);
		addEnemy(Spaceship::Enemy2, 0.f, 4200.f);

		addEnemy(Spaceship::Boss2, 0.f, 6000.f);
	}

	if (mLevel == 4) {
		
		endless();
	}

	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs) {
		return lhs.y < rhs.y;
	});
}

void World::endless() {
	int i = 0;
	while (!mPlayerAircraft->isDestroyed() && i < 10000) {
		i ++;
		int j = randomInt(20);
		addEnemy(Spaceship::Enemy2, j *  50.f, (i+j) * 1500.f);
		addEnemy(Spaceship::Enemy2, j * -50.f, (i+j) * 250.f);
		addEnemy(Spaceship::Enemy2, j * 50.f, (i+j) * 250.f);
		addEnemy(Spaceship::Enemy1, j * -50.f, (i+j) * 100.f);
		addEnemy(Spaceship::Enemy1, j * 50.f, (i+j) * 100.f );
		addEnemy(Spaceship::Enemy3, j * 100.f, (i+j) * 450.f);
		addEnemy(Spaceship::Enemy3, j * -100.f, (i+j) * 450.f);
		addEnemy(Spaceship::Enemy4, -500.f, (i+j) * 1000.f);
		addEnemy(Spaceship::Enemy5,  500.f, (i+j) * 1000.f);
		addEnemy(Spaceship::Enemy4, j * -150.f, (i + j) * 500.f);
		addEnemy(Spaceship::Enemy5, j * 150.f, (i + j) * 500.f);

	}
}

void World::updateTime(sf::Time& tIn) {
	mTime = tIn;
}

sf::Time World::getTime() {
	return mTime;
}

void World::addEnemy(Spaceship::Type type, float relX, float relY) {
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}


void World::spawnEnemies() {

	// Spawn all enemies entering the view area (including distance) this frame
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top) {
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Spaceship> enemy(new Spaceship(spawn.type, mTextures, mFonts, mPlayer));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);


		mSceneLayers[UpperAir]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView() {
	Command command;
	command.category = Category::Projectile | Category::EnemySpaceship;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time) {
		if (!getBattlefieldBounds().intersects(e.getRectBounds()))
			e.remove();
	});

	mCommandQueue.push(command);
}



void World::guideMissiles() {
	// Setup command that stores all enemies in mActiveEnemies
	Command enemyCollector;
	enemyCollector.category = Category::EnemySpaceship;
	enemyCollector.action = derivedAction<Spaceship>([this](Spaceship& enemy, sf::Time) {
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	// Setup command that guides all missiles to the enemy which is currently closest to the player
	Command missileGuider;
	missileGuider.category = Category::AlliedProjectile;
	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time) {
		// Ignore unguided bullets
		if (!missile.isGuided())
			return;

		float minDistance = std::numeric_limits<float>::max();
		Spaceship* closestEnemy = nullptr;

		// Find closest enemy
		for (Spaceship* enemy : mActiveEnemies) {
			float enemyDistance = distance(missile, *enemy);

			if (enemyDistance < minDistance) {
				closestEnemy = enemy;
				minDistance = enemyDistance;
			}
		}

		if (closestEnemy)
			missile.guideTowards(closestEnemy->getWorldPosition());
	});

	// Push commands, reset active enemies
	mCommandQueue.push(missileGuider);
	mCommandQueue.push(enemyCollector);

	mActiveEnemies.clear();
}

sf::FloatRect World::getViewBounds() const {
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const {
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::setLevel(int lin) {
	mLevel = lin;
}
