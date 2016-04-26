#ifndef GAME_PARTICLENODE_H
#define GAME_PARTICLENODE_H
#include <Game/SceneNode.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/Particle.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <deque>

class ParticleNode : public SceneNode {
public:
	ParticleNode(Particle::Type, const TextureHolder&);

	void addParticle(sf::Vector2f);
	Particle::Type getParticleType() const;
	virtual unsigned int getCategory() const;

private:
	virtual void updateCurrent(sf::Time, CommandQueue&);
	virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

	void addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
	void computeVertices() const;

	std::deque<Particle> mParticles;
	const sf::Texture& mTexture;
	Particle::Type mType;

	mutable sf::VertexArray	mVertexArray;
	mutable bool mNeedsVertexUpdate;
};
#endif // !GAME_PARTICLENODE_H