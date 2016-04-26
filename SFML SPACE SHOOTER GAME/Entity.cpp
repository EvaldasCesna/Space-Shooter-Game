#include <Game/Entity.h>
#include <cassert>
#include <iostream>
Entity::Entity(int health): mVelocity(), mHealth(health), mScore(0) {}

void Entity::setVelocity(sf::Vector2f velocity) {
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy) {
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const {
	return mVelocity;
}

void Entity::accelerate(sf::Vector2f velocity) {
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy) {
	mVelocity.x += vx;
	mVelocity.y += vy;
}

int Entity::getHitpoints() const {
	return mHealth;
}

void Entity::repair(int points) {
	assert(points > 0);
	mHealth += points;
}

void Entity::damage(int points) {
	assert(points > 0);
	mHealth -= points;
}

void Entity::destroy() {
	mHealth = 0;
}

void Entity::remove() {
	destroy();
}

bool Entity::isDestroyed() const {
	return mHealth <= 0;
}

void Entity::setScore(int in) {

	mScore += in;
}

int Entity::getScore() const {
	return mScore;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&) {
	move(mVelocity * dt.asSeconds());
}

