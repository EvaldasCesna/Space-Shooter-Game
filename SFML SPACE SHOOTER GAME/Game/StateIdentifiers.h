#ifndef GAME_STATEIDENTIFIERS_H
#define GAME_STATEIDENTIFIERS_H
namespace States {
	enum ID {
		None,
		Title,
		Menu,
		Game,
		Play,
		Loading,
		Pause,
		LevelSelect,
		Upgrade,
		Leader,
		Help,
		Settings,
		GameOver
	};
}
#endif // !GAME_STATEIDENTIFIERS_H