#ifndef MENU_LABEL_H
#define MENU_LABEL_H
#include <Menu/Component.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/Text.hpp>

namespace GUI {
class Label : public Component {
public:
	typedef std::shared_ptr<Label> Ptr;

	Label(const std::string&, const FontHolder&);
	virtual bool isSelectable() const;
	void setText(const std::string&);
	virtual void handleEvent(const sf::Event& event);

private:
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	sf::Text mText;

	};
}
#endif // !MENU_LABEL_H