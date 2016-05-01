#include <Game/Player.h>
#include <Game/CommandQueue.h>
#include <Game/Spaceship.h>
#include <map>
#include <string>
#include <algorithm>

#include <Game/Entity.h>
using namespace std::placeholders;

struct AircraftMover
{
	AircraftMover(float vx, float vy) : velocity(vx, vy) {}

	void operator() (Spaceship& spaceship, sf::Time) const {
		spaceship.accelerate(velocity * spaceship.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player() : mCurrentMissionStatus(LevelRunning)
{
	// Set keys
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::M] = Special;
	// Set actions
	initializeActions();

	resetScore();
	// Assign all categories to player's aircraft
	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerSpaceship;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
	if (event.type == sf::Event::KeyPressed) {
		// Commands happen when the player presses a key that corresponds with a command
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands) {
	// Checks every key for a press
	for (auto pair : mKeyBinding) {
		// if something is pressed make sure to trigger command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key) {
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); ) {
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}
	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const {
	for (auto pair : mKeyBinding) {
		if (pair.second == action)
			return pair.first;
	}
	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(LevelStatus status) {
	mCurrentMissionStatus = status;
}

Player::LevelStatus Player::getMissionStatus() const {
	return mCurrentMissionStatus;
}

void Player::initializeActions() {
	// The actions that are set according to a key
	mActionBinding[MoveLeft].action = derivedAction<Spaceship>(AircraftMover(-1, 0));
	mActionBinding[MoveRight].action = derivedAction<Spaceship>(AircraftMover(+1, 0));
	mActionBinding[MoveUp].action = derivedAction<Spaceship>(AircraftMover(0, -1));
	mActionBinding[MoveDown].action = derivedAction<Spaceship>(AircraftMover(0, +1));
	mActionBinding[Fire].action = derivedAction<Spaceship>([](Spaceship& a, sf::Time) { a.shoot(); });
	mActionBinding[Special].action = derivedAction<Spaceship>([](Spaceship& a, sf::Time) { a.shootMissile(); });
}

bool Player::isRealtimeAction(Action action) {
	// If its true it is a real action
	switch (action) {
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
	case Fire:
		return true;
	default:
		return false;
	}
}

void Player::updateScore(int in) {
	mScore = in;
}

int Player::getScore() { 
	return mScore;
}

void Player::resetScore() {
	mScore = 0;
}

void Player::setLevel(int l) {
	mLevel = l;
}

int Player::getLevel() {
	return mLevel;
}

void Player::setHull(int h) {
	mHull = h;
}

int Player::getHull(){
	return mHull;
}

void Player::setShip(int s) {
	mShip = s;
}

int Player::getShip() {
	return mShip;
}

void Player::setWeapon1(int w) {
	mWeapon1 = w;
}

int Player::getWeapon1() {
	return mWeapon1;
}

void Player::setWeapon2(int w) {
	mWeapon2 = w;
}

int Player::getWeapon2() {
	return mWeapon2;
}
