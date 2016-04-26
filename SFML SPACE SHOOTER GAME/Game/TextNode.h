#ifndef GAME_TEXTNODE_H
#define GAME_TEXTNODE_H
#include <Game/ResourceHolder.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/SceneNode.h>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode {
public:
	explicit TextNode(const FontHolder&, const std::string&);
	void setString(const std::string&);

private:
	virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
	sf::Text mText;
};
#endif // !GAME_TEXTNODE_H