#include <Game/Projectile.h>
#include <Game/EmitterNode.h>
#include <Game/DataTables.h>
#include <Game/Utility.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <cmath>
#include <cassert>

namespace {
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures) : Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
, mTargetDirection()
{
	centerOrigin(mSprite);
	// Adds particles for missiles
	if (isGuided()) {
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
		smoke->setPosition(0.f, getRectBounds().height / 2.f);
		attachChild(std::move(smoke));

		std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant));
		propellant->setPosition(0.f, getRectBounds().height / 2.f);
		attachChild(std::move(propellant));
	}
	if (isBigLaser()) {
		mSprite.scale(mSprite.getScale().x, mSprite.getScale().y + 20);
	}
}

void Projectile::guideTowards(sf::Vector2f position) {
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const {
	return mType == Missile;
}

bool Projectile::isBigLaser() const {
	return mType == BigLaser;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands) {
	if (isGuided()) {
		const float approachRate = 200.f;
		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}
	updateProjectilePattern(dt);

	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

unsigned int Projectile::getCategory() const {
	if (mType == EnemyLaser)
		return Category::EnemyProjectile;
	if (mType == EnemyLaser2) 
		return Category::EnemyProjectile;
	if (mType == EnemyLaser3)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getRectBounds() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const {
	return Table[mType].speed;
}

int Projectile::getDamage() const {
	return Table[mType].damage;
}

void Projectile::updateProjectilePattern(sf::Time dt) {
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
