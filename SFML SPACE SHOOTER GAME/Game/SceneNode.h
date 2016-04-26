#ifndef GAME_SCENENODE_H
#define GAME_SCENENODE_H
#include <Game/Category.h>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <set>
#include <memory>
#include <utility>
struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

	explicit SceneNode(Category::Type category = Category::None);

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

	void update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

	void onCommand(const Command&, sf::Time);
	virtual unsigned getCategory() const;

	void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
	void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
	void removeWrecks();
	virtual sf::FloatRect getRectBounds() const;
	virtual bool isMarkedForRemoval() const;
	virtual bool isDestroyed() const;

private:
	virtual void updateCurrent(sf::Time, CommandQueue&);
	void updateChildren(sf::Time, CommandQueue&);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
	void drawChildren(sf::RenderTarget&, sf::RenderStates) const;
	void drawBoundingRect(sf::RenderTarget&, sf::RenderStates) const;

	std::vector<Ptr> mChildren;
	SceneNode* mParent;
	Category::Type mDefaultCategory;
};
bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);
#endif // !GAME_SCENENODE_H