#ifndef GAME_PICKUP_H
#define GAME_PICKUP_H
#include <Game/Entity.h>
#include <Game/Command.h>
#include <Game/ResourceIdentifiers.h>
#include <SFML/Graphics/Sprite.hpp>
class Spaceship;

class Pickup : public Entity {
public:
	enum Type {
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TypeCount
	};

	Pickup(Type, const TextureHolder&);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getRectBounds() const;

	void apply(Spaceship& player) const;

protected:
	virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

private:
	Type mType;
	sf::Sprite mSprite;
};
#endif // !GAME_PICKUP_H
