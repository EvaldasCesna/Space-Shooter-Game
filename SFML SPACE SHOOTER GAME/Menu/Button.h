#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H
#include <Menu/Component.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/State.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <string>
#include <memory>
#include <functional>

class SoundPlayer;

namespace GUI {
	class Button : public Component {
	public:
		typedef std::shared_ptr<Button> Ptr;
		typedef std::function<void()> Callback;

		enum Type {
			Normal,
			Selected,
			Pressed,
			ButtonCount
		};

		Button(State::Context);

		//put level select here maybe

		void setCallback(Callback);
		void setText(const std::string&);
		void setToggle(bool);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void activate();
		virtual void deactivate();

		virtual void handleEvent(const sf::Event& event);

	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		void changeTexture(Type);

		Callback mCallback;
		sf::Sprite mSprite;
		sf::Text mText;
		bool mIsToggle;
		SoundPlayer& mSounds;
	};
}
#endif // !MENU_BUTTON_H