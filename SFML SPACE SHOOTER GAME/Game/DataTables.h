#ifndef GAME_DATATABLES_H
#define GAME_DATATABLES_H
#include <Game/ResourceIdentifiers.h>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <functional>
#include <Game/Player.h>
class Spaceship;

struct Direction {
	Direction(float angle, float distance) : angle(angle), distance(distance) {}
	float angle;
	float distance;
};

struct AircraftData {
	int health;
	int mScore;
	float speed;
	Textures::ID texture;
	sf::IntRect textureRect;
	sf::Time fireInterval;
	std::vector<Direction> directions;
	bool hasAnimation;
};

struct ProjectileData {
	int damage;
	float speed;
	Textures::ID texture;
	std::vector<Direction> directions;
	sf::IntRect textureRect;
};

struct PickupData {
	std::function<void(Spaceship&)>	action;
	Textures::ID texture;
	sf::IntRect textureRect;
};

struct ParticleData {
	sf::Color color;
	sf::Time lifetime;
};

std::vector<AircraftData>	initializeAircraftData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<PickupData>		initializePickupData();
std::vector<ParticleData>	initializeParticleData();

#endif // !GAME_DATATABLES_H