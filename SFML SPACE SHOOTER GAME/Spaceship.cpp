#include <Game/Spaceship.h>
#include <Game/DataTables.h>
#include <Game/Utility.h>
#include <Game/Pickup.h>
#include <Game/CommandQueue.h>
#include <Sound/SoundNode.h>
#include <Game/ResourceHolder.h>
#include <Game/Player.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <cmath>

using namespace std::placeholders;

namespace {

	const std::vector<AircraftData> Table = initializeAircraftData();

}
Spaceship::Spaceship(Type type, const TextureHolder& textures, const FontHolder& fonts, Player& player) : Entity(Table[type].health)
, mType(type)
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
, mExplosion(textures.get(Textures::Explosion))
, mFireCommand(), mWeaponCommand(), mFireCountdown(sf::Time::Zero)
, mPlayer(player)
, mIsFiring(false), mIsLaunchingWeapon(false)
, mShowExplosion(true), mPlayedExplosionSound(false)
, mSpawnedPickup(false)
, mFireRateLevel(1), mSpreadLevel(1), mWeaponAmmo(2)
, mDropPickupCommand()
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mHealthDisplay(nullptr), mWeaponDisplay(nullptr)
{

	mExplosion.setFrameSize(sf::Vector2i(100, 100));
	mExplosion.setNumFrames(20);
	mExplosion.setDuration(sf::seconds(1));

	centerOrigin(mSprite);
	centerOrigin(mExplosion);

	mFireCommand.category = Category::SceneSpaceLayer;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time) {
		createLasers(node, textures);
	};

	if (CheckWeapon() == Projectile::Missile) {
		mWeaponCommand.category = Category::SceneSpaceLayer;
		mWeaponCommand.action = [this, &textures](SceneNode& node, sf::Time) {
			createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
		};
	}

	if (CheckWeapon() == Projectile::BigLaser) {
		mWeaponCommand.category = Category::SceneSpaceLayer;
		mWeaponCommand.action = [this, &textures](SceneNode& node, sf::Time) {
			createProjectile(node, Projectile::BigLaser, 0.f, 6.f, textures);
		};
	}

	if (type == Type::Boss) {

			mSpreadLevel = 6;
	
	}

	if (type == Type::Boss2) {

		mSpreadLevel = 7;

	}

	mDropPickupCommand.category = Category::SceneSpaceLayer;
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time) {
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (getCategory() == Category::PlayerSpaceship) {
		setShip();

		std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		missileDisplay->setPosition(0, 70);
		mWeaponDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));

		std::unique_ptr<TextNode> scoreDisplay(new TextNode(fonts, ""));
		scoreDisplay->setPosition(0, 90);
		mScoredisplay = scoreDisplay.get();
		attachChild(std::move(scoreDisplay));
	}
	updateTexts();
}

void Spaceship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Spaceship::updateCurrent(sf::Time dt, CommandQueue& commands) {
	// Update text and animation
	updateTexts();
	updateAnimation();

	// Entity is destroyed: maybe drop pickup, mark for removal
	if (isDestroyed()) {
		checkPickupDrop(commands);
		mExplosion.update(dt);

		// Play explosion sound only once
		if (!mPlayedExplosionSound) {
			SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
			playLocalSound(commands, soundEffect);

			mPlayedExplosionSound = true;
		}
		return;
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	
	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);
}

unsigned int Spaceship::getCategory() const {
	if (isAllied())
		return Category::PlayerSpaceship;
	else
		return Category::EnemySpaceship;
}

sf::FloatRect Spaceship::getRectBounds() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Spaceship::isMarkedForRemoval() const {
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void Spaceship::remove() {
	Entity::remove();
	mShowExplosion = false;
}

bool Spaceship::isAllied() const {
	if (mPlayer.getHull() == 1) {
		return mType == Player1;
	}

	if (mPlayer.getHull() == 2) {
		return mType == Player2;
	}
	else {
		return mType == Player1;
	}
}

void Spaceship::setShip() {
	if (mPlayer.getShip() == 1) {
		mSprite.setColor(setHue(255, 255, 255));
	}
	if (mPlayer.getShip() == 2) {
		mSprite.setColor(setHue(0, 0, 255));
	}
	if (mPlayer.getShip() == 3) {
		mSprite.setColor(setHue(0, 255, 0));
	}
}

float Spaceship::getMaxSpeed() const {
	return Table[mType].speed;
}

void Spaceship::increaseFireRate() {
	if (CheckLaser() == Projectile::AlliedLaser) {
		if (mFireRateLevel < 4)
			++mFireRateLevel;
	}
	if (CheckLaser() == Projectile::AlliedLaser2) {
		if (mFireRateLevel < 2)
			++mFireRateLevel;
	}
}

void Spaceship::increaseSpread() {
	if (CheckLaser() == Projectile::AlliedLaser) {
		if (mSpreadLevel < 3)
			++mSpreadLevel;
	}
	if (CheckLaser() == Projectile::AlliedLaser2) {
		if (mSpreadLevel < 2)
			++mSpreadLevel;
	}
}

void Spaceship::collectMissiles(unsigned int count) {
	mWeaponAmmo += count;
}

void Spaceship::shoot() {
	// ships with fire interval != 0 are able to fire
	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}

void Spaceship::shootMissile() {
	if (mWeaponAmmo > 0) {
		mIsLaunchingWeapon = true;
		--mWeaponAmmo;
	}
}

void Spaceship::playLocalSound(CommandQueue& commands, SoundEffect::ID effect) {
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>([effect, worldPosition](SoundNode& node, sf::Time) {
		node.playSound(effect, worldPosition);
	});

	commands.push(command);
}

void Spaceship::updateMovementPattern(sf::Time dt) {
	// Enemy movements
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty()) {
		// Moves then changes direction
		if (mTravelledDistance > directions[mDirectionIndex].distance) {
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}
		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Spaceship::checkPickupDrop(CommandQueue& commands) {
	// Every so often it drops a pickup
	if (!isAllied() && randomInt(3) == 0 && !mSpawnedPickup)
		commands.push(mDropPickupCommand);

	mSpawnedPickup = true;
}

void Spaceship::checkProjectileLaunch(sf::Time dt, CommandQueue& commands) {
	// Enemies shoot all the time
	if (!isAllied())
		shoot();
	// Automatically shoots in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero) {
		// Every interval shoot
		
		commands.push(mFireCommand);
		playLocalSound(commands, isAllied() ? SoundEffect::AlliedLaser : SoundEffect::EnemyLaser);

		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero) {
		// Interval not expired wait
		mFireCountdown -= dt;
		mIsFiring = false;
	}
	// Check if missile is launched
	if (mIsLaunchingWeapon) {
		commands.push(mWeaponCommand);
		playLocalSound(commands, SoundEffect::Special);

		mIsLaunchingWeapon = false;
	}
}

Projectile::Type Spaceship::CheckLaser() const {
	if (mPlayer.getWeapon1() == 1) {
		return Projectile::AlliedLaser;
	}
	if (mPlayer.getWeapon1() == 2) {
		return Projectile::AlliedLaser2;
	}
	else {
		return Projectile::AlliedLaser;
	}
}

Projectile::Type Spaceship::CheckEnemyLaser() const {
	if (mType == Type::Boss2) {
		return Projectile::EnemyLaser3;
	}
	if (mType == Type::Boss) {
		return Projectile::EnemyLaser2;
	}
	else {
		return Projectile::EnemyLaser;
	}
}

Projectile::Type Spaceship::CheckWeapon() const {
	if (mPlayer.getWeapon2() == 1) {
		return Projectile::Missile;
	}
	if (mPlayer.getWeapon2() == 2) {
		return Projectile::BigLaser;
	}
	else {
		return Projectile::Missile;
	}

}

void Spaceship::createLasers(SceneNode& node, const TextureHolder& textures) const {
	Projectile::Type weapon = CheckLaser();
	Projectile::Type enemyweapon = CheckEnemyLaser();

	Projectile::Type type = isAllied() ? weapon : enemyweapon;

		// More lasers per level
		switch (mSpreadLevel) {
		case 1:
			createProjectile(node, type, 0.0f, 0.5f, textures);
			break;

		case 2:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;

		case 3:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;
		case 6:
			createProjectile(node, type, -1.f, 0.33f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +1.f, 0.33f, textures);
			break;
		case 7:
			createProjectile(node, type, -0.32f, 0.30f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +0.32f, 0.30f, textures);
			break;
		}
}

void Spaceship::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const {
	
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);

	node.attachChild(std::move(projectile));
}

void Spaceship::createPickup(SceneNode& node, const TextureHolder& textures) const {
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}

void Spaceship::updateTexts() {
	// Display hitpoints
	if (isDestroyed()) {
		mHealthDisplay->setString("");
	}
	else
	mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());

	// Display missiles, if available
	if (mWeaponDisplay) {
		if (isDestroyed())
			mWeaponDisplay->setString("");
		else
			mWeaponDisplay->setString("W: " + std::to_string(mWeaponAmmo));
	}

	if (mScoredisplay) {

		if (isDestroyed()) {
			mScoredisplay->setString("");
		}
		else {
			mScoredisplay->setString("Score: " + std::to_string(getScore()));
		}
	}
}

void Spaceship::updateAnimation() {
	if (Table[mType].hasAnimation) {
		sf::IntRect textureRect = Table[mType].textureRect;

		// Left: Texture rect offset once
		if (getVelocity().x < 0.f)
			textureRect.left += textureRect.width;

		// Right: Texture rect offset twice
		else if (getVelocity().x > 0.f)
			textureRect.left += 2 * textureRect.width;

		mSprite.setTextureRect(textureRect);
	}
}
