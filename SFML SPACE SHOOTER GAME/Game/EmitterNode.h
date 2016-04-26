#ifndef GAME_EMMITERNODE_H
#define GAME_EMMITERNODE_H
#include <Game/SceneNode.h>
#include <Game/Particle.h>
class ParticleNode;

class EmitterNode : public SceneNode {
public:
	explicit EmitterNode(Particle::Type);

private:
	virtual void updateCurrent(sf::Time, CommandQueue&);
	void emitParticles(sf::Time);
	sf::Time mAccumulatedTime;
	Particle::Type mType;
	ParticleNode* mParticleSystem;
};
#endif // !GAME_EMMITERNODE_H