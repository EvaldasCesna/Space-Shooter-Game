#ifndef SOUND_SOUNDNODE_H
#define SOUND_SOUNDNODE_H
#include <Game/SceneNode.h>
#include <Game/ResourceIdentifiers.h>
class SoundPlayer;

class SoundNode : public SceneNode {
public:
	explicit SoundNode(SoundPlayer&);
	void playSound(SoundEffect::ID sound, sf::Vector2f position);

	virtual unsigned int getCategory() const;

private:
	SoundPlayer& mSounds;
};
#endif // !SOUND_SOUNDNODE_H