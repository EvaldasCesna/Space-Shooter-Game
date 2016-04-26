#include <Game/Pickup.h>
#include <Game/DataTables.h>
#include <Game/Category.h>
#include <Game/CommandQueue.h>
#include <Game/Utility.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderTarget.hpp>

namespace {
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures) : Entity(1), mType(type), mSprite(textures.get(Table[type].texture), Table[type].textureRect) {
	centerOrigin(mSprite);
}

unsigned int Pickup::getCategory() const {
	return Category::Pickup;
}

sf::FloatRect Pickup::getRectBounds() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Spaceship& player) const {
	Table[mType].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

