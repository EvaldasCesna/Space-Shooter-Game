#ifndef MENU_CONTAINER_H
#define MENU_CONTAINER_H
#include <Menu/Component.h>
#include <vector>
#include <memory>

namespace GUI {
class Container : public Component {
public:
	typedef std::shared_ptr<Container> Ptr;         
	Container();

	void pack(Component::Ptr);
	virtual bool isSelectable() const;
	virtual void handleEvent(const sf::Event&);

private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	bool hasSelection() const;
	void select(std::size_t);
	void selectNext();
	void selectPrevious();
	std::vector<Component::Ptr> mChildren;
	int mSelectedChild;
	};
}
#endif // !MENU_CONTAINER_H