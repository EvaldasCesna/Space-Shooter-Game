#ifndef GAME_BLOOMEFFECT_H
#define GAME_BLOOMEFFECT_H
#include <Game/PostEffect.h>
#include <Game/ResourceIdentifiers.h>
#include <Game/ResourceHolder.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <array>

class BloomEffect : public PostEffect {
public:
	BloomEffect();
	virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output);

private:
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

	void	prepareTextures(sf::Vector2u);
	void	filterBright(const sf::RenderTexture&, sf::RenderTexture&);
	void	blurMultipass(RenderTextureArray&);
	void	blur(const sf::RenderTexture&, sf::RenderTexture&, sf::Vector2f);
	void	downsample(const sf::RenderTexture&, sf::RenderTexture&);
	void	add(const sf::RenderTexture&, const sf::RenderTexture&, sf::RenderTarget&);

	ShaderHolder mShaders;
	sf::RenderTexture	mBrightnessTexture;
	RenderTextureArray	mFirstPassTextures;
	RenderTextureArray	mSecondPassTextures;
};
#endif // !GAME_BLOOMEFFECT_H
