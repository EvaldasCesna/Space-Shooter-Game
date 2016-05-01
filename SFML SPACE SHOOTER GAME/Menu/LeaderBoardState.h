#ifndef MENU_LEADERBOARDSTATE_H
#define MENU_LEADERBOARDSTATE_H
#include <Game/State.h>
#include <Menu/Container.h>
#include <Menu/Button.h>
#include <Menu/Label.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Database/Database.h>

class LeaderBoardState : public State {
public:
	LeaderBoardState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
	GUI::Label::Ptr Scores;
	void UpdateLeaderboard();

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	
};
#endif // !MENU_LEADERBOARDSTATE_H