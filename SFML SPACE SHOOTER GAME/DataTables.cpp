#include <Game/DataTables.h>
#include <Game/Spaceship.h>
#include <Game/Projectile.h>
#include <Game/Pickup.h>
#include <Game/Particle.h>
using namespace std::placeholders; // _1, _2 etc

std::vector<AircraftData> initializeAircraftData() {
	std::vector<AircraftData> data(Spaceship::TypeCount);

	data[Spaceship::Player1].health = 100;
	data[Spaceship::Player1].mScore = 0;
	data[Spaceship::Player1].speed = 500.f;
	data[Spaceship::Player1].fireInterval = sf::seconds(0.8);
	data[Spaceship::Player1].texture = Textures::Player;
	data[Spaceship::Player1].textureRect = sf::IntRect(0, 0, 53, 52);
	data[Spaceship::Player1].hasAnimation = true;

	data[Spaceship::Player2].health = 80;
	data[Spaceship::Player2].mScore = 0;
	data[Spaceship::Player2].speed = 400.f;
	data[Spaceship::Player2].fireInterval = sf::seconds(0.8);
	data[Spaceship::Player2].texture = Textures::Player;
	data[Spaceship::Player2].textureRect = sf::IntRect(0, 0, 53, 52);
	data[Spaceship::Player2].hasAnimation = true;

	data[Spaceship::Enemy1].health = 20;
	data[Spaceship::Enemy1].speed = 100.f;
	data[Spaceship::Enemy1].texture = Textures::Enemy;
	data[Spaceship::Enemy1].textureRect = sf::IntRect(0, 0, 70, 66);
	data[Spaceship::Enemy1].directions.push_back(Direction(+45.f, 80.f));
	data[Spaceship::Enemy1].directions.push_back(Direction(-45.f, 160.f));
	data[Spaceship::Enemy1].directions.push_back(Direction(+45.f, 80.f));
	data[Spaceship::Enemy1].fireInterval = sf::Time::Zero;
	data[Spaceship::Enemy1].hasAnimation = false;

	data[Spaceship::Enemy2].health = 40;
	data[Spaceship::Enemy2].speed = 60.f;
	data[Spaceship::Enemy2].texture = Textures::Enemy;
	data[Spaceship::Enemy2].textureRect = sf::IntRect(70, 0, 58, 100);
	data[Spaceship::Enemy2].directions.push_back(Direction(+45.f, 50.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(0.f, 50.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(-45.f, 100.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(0.f, 50.f));
	data[Spaceship::Enemy2].directions.push_back(Direction(+45.f, 50.f));
	data[Spaceship::Enemy2].fireInterval = sf::seconds(1.5);
	data[Spaceship::Enemy2].hasAnimation = false;

	data[Spaceship::Enemy3].health = 100;
	data[Spaceship::Enemy3].speed = 30.f;
	data[Spaceship::Enemy3].texture = Textures::Enemy;
	data[Spaceship::Enemy3].textureRect = sf::IntRect(128, 0, 65, 84);
	data[Spaceship::Enemy3].directions.push_back(Direction(0.f, 100.f));
	data[Spaceship::Enemy3].fireInterval = sf::Time::Zero;
	data[Spaceship::Enemy3].hasAnimation = false;

	data[Spaceship::Enemy4].health = 15;
	data[Spaceship::Enemy4].speed = 100.f;
	data[Spaceship::Enemy4].texture = Textures::Enemy;
	data[Spaceship::Enemy4].textureRect = sf::IntRect(194, 0, 65, 70);
	data[Spaceship::Enemy4].directions.push_back(Direction(-65.f, 50.f));
	data[Spaceship::Enemy4].fireInterval = sf::seconds(2);
	data[Spaceship::Enemy4].hasAnimation = false;

	data[Spaceship::Enemy5].health = 15;
	data[Spaceship::Enemy5].speed = 100.f;
	data[Spaceship::Enemy5].texture = Textures::Enemy;
	data[Spaceship::Enemy5].textureRect = sf::IntRect(194, 0, 65, 70);
	data[Spaceship::Enemy5].directions.push_back(Direction(+65.f, 50.f));
	data[Spaceship::Enemy5].fireInterval = sf::seconds(2);
	data[Spaceship::Enemy5].hasAnimation = false;

	data[Spaceship::Boss].health = 1500;
	data[Spaceship::Boss].speed = 50.f;
	data[Spaceship::Boss].texture = Textures::Boss;
	data[Spaceship::Boss].textureRect = sf::IntRect(0, 0, 300, 270);
	data[Spaceship::Boss].directions.push_back(Direction(+160.f, 300.f));
	data[Spaceship::Boss].directions.push_back(Direction(-160.f, 600.f));
	data[Spaceship::Boss].directions.push_back(Direction(+160.f, 300.f));
	data[Spaceship::Boss].fireInterval = sf::seconds(4.5);
	data[Spaceship::Boss].hasAnimation = false;

	data[Spaceship::Boss2].health = 1500;
	data[Spaceship::Boss2].speed = 50.f;
	data[Spaceship::Boss2].texture = Textures::Boss;
	data[Spaceship::Boss2].textureRect = sf::IntRect(221, 217, 380, 380);
	data[Spaceship::Boss2].directions.push_back(Direction(+160.f, 400.f));
	data[Spaceship::Boss2].directions.push_back(Direction(-160.f, 700.f));
	data[Spaceship::Boss2].directions.push_back(Direction(+160.f, 400.f));
	data[Spaceship::Boss2].fireInterval = sf::seconds(4);
	data[Spaceship::Boss2].hasAnimation = false;

	return data;
}

std::vector<ProjectileData> initializeProjectileData() {
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedLaser].damage = 10;
	data[Projectile::AlliedLaser].speed = 1250.f;
	data[Projectile::AlliedLaser].texture = Textures::Shots;
	data[Projectile::AlliedLaser].textureRect = sf::IntRect(0, 0, 5, 13);

	data[Projectile::AlliedLaser2].damage = 15;
	data[Projectile::AlliedLaser2].speed = 1200.f;
	data[Projectile::AlliedLaser2].texture = Textures::Shots;
	data[Projectile::AlliedLaser2].textureRect = sf::IntRect(16, 2,10, 10);
	data[Projectile::AlliedLaser2].directions.push_back(Direction(+190.f, 180.f));
	data[Projectile::AlliedLaser2].directions.push_back(Direction(-190.f, 180.f));

	data[Projectile::EnemyLaser].damage = 10;
	data[Projectile::EnemyLaser].speed = 300.f;
	data[Projectile::EnemyLaser].texture = Textures::Shots;
	data[Projectile::EnemyLaser].textureRect = sf::IntRect(6, 0, 5, 13);

	data[Projectile::EnemyLaser2].damage = 5;
	data[Projectile::EnemyLaser2].speed = 400.f;
	data[Projectile::EnemyLaser2].texture = Textures::Shots;
	data[Projectile::EnemyLaser2].textureRect = sf::IntRect(28, 0,11, 11);
	data[Projectile::EnemyLaser2].directions.push_back(Direction(+90.f, 80.f));
	data[Projectile::EnemyLaser2].directions.push_back(Direction(-90.f, 80.f));

	data[Projectile::EnemyLaser3].damage = 5;
	data[Projectile::EnemyLaser3].speed = 400.f;
	data[Projectile::EnemyLaser3].texture = Textures::Shots;
	data[Projectile::EnemyLaser3].textureRect = sf::IntRect(28, 0, 11, 11);
	data[Projectile::EnemyLaser3].directions.push_back(Direction(+80.f,180.f));
	data[Projectile::EnemyLaser3].directions.push_back(Direction(-80.f, 180.f));

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Shots;
	data[Projectile::Missile].textureRect = sf::IntRect(0, 13, 16, 33);

	data[Projectile::BigLaser].damage = 2;
	data[Projectile::BigLaser].speed = 1100.f;
	data[Projectile::BigLaser].texture = Textures::Shots;
	data[Projectile::BigLaser].textureRect = sf::IntRect(17, 13, 12, 33);

	return data;
}

std::vector<PickupData> initializePickupData() {
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::Entities;
	data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 0, 40, 40);
	data[Pickup::HealthRefill].action = [](Spaceship& a) { a.repair(15); };

	data[Pickup::MissileRefill].texture = Textures::Entities;
	data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 0, 40, 40);
	data[Pickup::MissileRefill].action = std::bind(&Spaceship::collectMissiles, _1, 2);

	data[Pickup::FireSpread].texture = Textures::Entities;
	data[Pickup::FireSpread].textureRect = sf::IntRect(80, 0, 40, 40);
	data[Pickup::FireSpread].action = std::bind(&Spaceship::increaseSpread, _1);

	data[Pickup::FireRate].texture = Textures::Entities;
	data[Pickup::FireRate].textureRect = sf::IntRect(120, 0, 40, 40);
	data[Pickup::FireRate].action = std::bind(&Spaceship::increaseFireRate, _1);

	return data;
}

std::vector<ParticleData> initializeParticleData() {
	std::vector<ParticleData> data(Particle::ParticleCount);

	data[Particle::Propellant].color = sf::Color(255, 200, 50);
	data[Particle::Propellant].lifetime = sf::seconds(0.6f);

	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].lifetime = sf::seconds(2.f);

	return data;
}
