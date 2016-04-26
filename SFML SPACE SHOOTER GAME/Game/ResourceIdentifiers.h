#ifndef GAME_RESOURCEIDENTIFIERS_H
#define GAME_RESOURCEIDENTIFIERS_H
// Declare all the SFML classes
namespace sf {
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

namespace Textures {
	enum ID {
		Entities,
		Player,
		Enemy,
		Shots,
		Pickup,
		Space,
		Space2,
		TitleScreen,
		Help,
		Buttons,
		Explosion,
		Particle,
		Boss,
		//	Enviroment,
	};
}

namespace Shaders {
	enum ID {
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};
}

namespace Fonts {
	enum ID {
		Main,
	};
}

namespace SoundEffect {
	enum ID {
		AlliedLaser,
		EnemyLaser,
		Explosion1,
		Explosion2,
		LaunchMissile,
		CollectPickup,
		Button,
	};
}

namespace Music {
	enum ID {
		MenuTheme,
		MissionTheme,
	};
}
// Forwards the declarations and type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;
#endif // !GAME_RESOURCEIDENTIFIERS_H