#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H
#include <Game/SceneNode.h>
#include <Game/Player.h>

class Entity : public SceneNode {
public:
	explicit Entity(int);
	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);
	sf::Vector2f getVelocity() const;
	int getHitpoints() const;
	void repair(int);
	void damage(int);
	void destroy();
	virtual void remove();
	virtual bool isDestroyed() const;
	
	void setScore(int);
	int getScore() const;

protected:
	virtual void updateCurrent(sf::Time, CommandQueue&);

private:
	sf::Vector2f mVelocity;
	int mHealth;
	int mScore;
};
#endif // !GAME_ENTITY_H