#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <Game/StateIdentifiers.h>
#include <Game/ResourceIdentifiers.h>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace sf {
	class RenderWindow;
}
class StateStack;
class Player;
class MusicPlayer;
class SoundPlayer;

class State {
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context {
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player,
			MusicPlayer& music, SoundPlayer& sounds);

		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Player* player;
		MusicPlayer* music;
		SoundPlayer* sounds;
	};

	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(States::ID);
	void requestStackPop();
	void requestStateClear();

	Context getContext() const;

private:
	StateStack* mStack;
	Context mContext;
};
#endif // !GAME_STATE_H