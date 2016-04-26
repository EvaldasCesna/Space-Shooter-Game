#ifndef GAME_CATEGORY_H
#define GAME_CATEGORY_H
// Each category is a group of one or multiple game objects
namespace Category {
	enum Type {
		None = 0,
		SceneAirLayer = 1 << 0,
		PlayerSpaceship = 1 << 1,
		AlliedSpaceship = 1 << 2,
		EnemySpaceship = 1 << 3,
		Pickup = 1 << 4,
		AlliedProjectile = 1 << 5,
		EnemyProjectile = 1 << 6,
		ParticleSystem = 1 << 7,
		SoundEffect = 1 << 8,

		Spaceship = PlayerSpaceship | AlliedSpaceship | EnemySpaceship,
		Projectile = AlliedProjectile | EnemyProjectile,
	};
}
#endif // !GAME_CATEGORY_H