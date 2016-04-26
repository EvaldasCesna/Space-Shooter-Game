#include <Book/Entity.hpp>
#include <Book/Command.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/Projectile.hpp>
#include <Book/TextNode.hpp>
#include <Book/Animation.hpp>

#include <SFML/Graphics/Sprite.hpp>


class Spaceship : public Entity
{
public:
	enum Type {
		Player1,
		Enemy1,
		Enemy2,
		Enemy3,
		Boss,
		TypeCount
	};
	Spaceship(Type, const TextureHolder&, const FontHolder&);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getRectBounds() const;
	virtual void remove();
	virtual bool isMarkedForRemoval() const;
	bool isAllied() const; // Checks which ship is the player
	float getMaxSpeed() const;
	void increaseFireRate();
	void increaseSpread();
	void collectMissiles(unsigned int);
	void shoot();
	void shootMissile();
	void playLocalSound(CommandQueue&, SoundEffect::ID);

private:
	virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
	virtual void updateCurrent(sf::Time, CommandQueue&);
	void updateMovementPattern(sf::Time);
	void checkPickupDrop(CommandQueue&);
	void checkProjectileLaunch(sf::Time, CommandQueue&);
	void createBullets(SceneNode&, const TextureHolder&) const;
	void createProjectile(SceneNode&, Projectile::Type, float, float, const TextureHolder&) const;
	void createPickup(SceneNode&, const TextureHolder&) const;
	void updateTexts();
	void updateAnimation();

	Type mType;
	sf::Sprite mSprite;
	Animation mExplosion;
	Command mFireCommand;
	Command	mMissileCommand;
	Command mDropPickupCommand;
	sf::Time mFireCountdown;
	bool mIsFiring;
	bool mIsLaunchingMissile;
	bool mShowExplosion;
	bool mPlayedExplosionSound;
	bool mSpawnedPickup;
	int	mFireRateLevel;
	int	mSpreadLevel;
	int	mMissileAmmo;
	int mScore;
	float mTravelledDistance;
	std::size_t mDirectionIndex;
	TextNode* mHealthDisplay;
	TextNode* mMissileDisplay;
	TextNode* mScoredisplay;
};

