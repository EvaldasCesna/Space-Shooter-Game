#ifndef GAME_PROJECTILE_H
#define GAME_PROJECTILE_H
#include <Game/Entity.h>
#include <Game/ResourceIdentifiers.h>
#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity {
public:
	enum Type {
		AlliedLaser,
		AlliedLaser2,
		EnemyLaser,
		EnemyLaser2,
		EnemyLaser3,
		Missile,
		BigLaser,
		TypeCount
	};
	Projectile(Type type, const TextureHolder& textures);

	void guideTowards(sf::Vector2f position);
	bool isGuided() const;
	bool isBigLaser() const;

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getRectBounds() const;
	float getMaxSpeed() const;
	int getDamage() const;
	void updateProjectilePattern(sf::Time dt);

private:

	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
	std::size_t mDirectionIndex;
	float mTravelledDistance;
	Type mType;
	sf::Sprite mSprite;
	sf::Vector2f mTargetDirection;
};
#endif // !GAME_PROJECTILE_H