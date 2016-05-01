#ifndef GAME_SPACESHIP_H
#define GAME_SPACESHIP_H
#include <Game/Entity.h>
#include <Game/Command.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/Projectile.h>
#include <Game/TextNode.h>
#include <Game/Animation.h>
#include <SFML/Graphics/Sprite.hpp>

class Spaceship : public Entity {
public:
	// The types of friendly and enemy
	enum Type {
		Player1,
		Player2,
		Enemy1,
		Enemy2,
		Enemy3,
		Enemy4,
		Enemy5,
		Boss,
		Boss2,
		TypeCount
	};
	Spaceship(Type, const TextureHolder&, const FontHolder&, Player&);

	virtual unsigned int getCategory() const;
	// The bounds of each image
	virtual sf::FloatRect getRectBounds() const;	
	// Gets rid of the dead enemy
	virtual void remove();				
	virtual bool isMarkedForRemoval() const;
	// Checks which ship is the player
	bool isAllied() const;
	void setShip();
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
	Projectile::Type CheckLaser() const;
	Projectile::Type CheckEnemyLaser() const;
	Projectile::Type CheckWeapon() const;
	void createLasers(SceneNode&, const TextureHolder&) const;
	void createProjectile(SceneNode&, Projectile::Type, float, float, const TextureHolder&) const;
	void createPickup(SceneNode&, const TextureHolder&) const;
	void updateTexts();
	void updateAnimation();

	Player& mPlayer;

	// holds The type of Spaceship 
	Type mType;	
	// holds a sprite
	sf::Sprite mSprite; 
	// The animation that we see for the explosion.
	Animation mExplosion;		
	Command mFireCommand;
	Command	mWeaponCommand;
	Command mDropPickupCommand;
	sf::Time mFireCountdown;
	bool mIsFiring;
	bool mIsLaunchingWeapon;
	bool mShowExplosion;
	bool mPlayedExplosionSound;
	bool mSpawnedPickup;
	int	mFireRateLevel;
	int	mSpreadLevel;
	int	mWeaponAmmo;
	float mTravelledDistance;
	std::size_t mDirectionIndex;
	TextNode* mHealthDisplay;
	TextNode* mWeaponDisplay;
	TextNode* mScoredisplay;


};
#endif // !GAME_SPACESHIP_H