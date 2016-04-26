#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class Animation : public sf::Drawable, public sf::Transformable {
public:
	Animation();
	explicit Animation(const sf::Texture&);

	void setTexture(const sf::Texture&);
	const sf::Texture* getTexture() const;

	void setFrameSize(sf::Vector2i);
	sf::Vector2i getFrameSize() const;

	void setNumFrames(std::size_t);
	std::size_t getNumFrames() const;

	void setDuration(sf::Time);
	sf::Time getDuration() const;

	void setRepeating(bool);
	bool isRepeating() const;

	void restart();
	bool isFinished() const;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	void update(sf::Time);
private:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;
	std::size_t mNumFrames;
	std::size_t mCurrentFrame;
	sf::Time mDuration;
	sf::Time mElapsedTime;
	bool mRepeat;
};
#endif // !GAME_ANIMATION_H