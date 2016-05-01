#include <Game/BloomEffect.h>

BloomEffect::BloomEffect() : mShaders(), mBrightnessTexture(), mFirstPassTextures(), mSecondPassTextures() {
	mShaders.load(Shaders::BrightnessPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/Brightness.frag");
	mShaders.load(Shaders::DownSamplePass, "Media/Shaders/Fullpass.vert", "Media/Shaders/DownSample.frag");
	mShaders.load(Shaders::GaussianBlurPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/GuassianBlur.frag");
	mShaders.load(Shaders::AddPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/Add.frag");
}

void BloomEffect::apply(const sf::RenderTexture& in, sf::RenderTarget& out) {
	prepareTextures(in.getSize());

	filterBright(in, mBrightnessTexture);

	downsample(mBrightnessTexture, mFirstPassTextures[0]);
	blurMultipass(mFirstPassTextures);
	downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
	blurMultipass(mSecondPassTextures);

	add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
	mFirstPassTextures[1].display();
	add(in, mFirstPassTextures[1], out);
}

void BloomEffect::prepareTextures(sf::Vector2u size) {
	if (mBrightnessTexture.getSize() != size) {
		mBrightnessTexture.create(size.x, size.y);
		mBrightnessTexture.setSmooth(true);

		mFirstPassTextures[0].create(size.x / 2, size.y / 2);
		mFirstPassTextures[0].setSmooth(true);
		mFirstPassTextures[1].create(size.x / 2, size.y / 2);
		mFirstPassTextures[1].setSmooth(true);

		mSecondPassTextures[0].create(size.x / 4, size.y / 4);
		mSecondPassTextures[0].setSmooth(true);
		mSecondPassTextures[1].create(size.x / 4, size.y / 4);
		mSecondPassTextures[1].setSmooth(true);
	}
}

void BloomEffect::filterBright(const sf::RenderTexture& in, sf::RenderTexture& out) {
	sf::Shader& brightness = mShaders.get(Shaders::BrightnessPass);

	brightness.setParameter("source", in.getTexture());
	applyShader(brightness, out);
	out.display();
}

void BloomEffect::blurMultipass(RenderTextureArray& renderTextures) {
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count) {
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void BloomEffect::blur(const sf::RenderTexture& in, sf::RenderTexture& out, sf::Vector2f offsetFactor) {
	sf::Shader& gaussianBlur = mShaders.get(Shaders::GaussianBlurPass);

	gaussianBlur.setParameter("source", in.getTexture());
	gaussianBlur.setParameter("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, out);
	out.display();
}

void BloomEffect::downsample(const sf::RenderTexture& in, sf::RenderTexture& out) {
	sf::Shader& downSampler = mShaders.get(Shaders::DownSamplePass);

	downSampler.setParameter("source", in.getTexture());
	downSampler.setParameter("sourceSize", sf::Vector2f(in.getSize()));
	applyShader(downSampler, out);
	out.display();
}

void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& out) {
	sf::Shader& adder = mShaders.get(Shaders::AddPass);

	adder.setParameter("source", source.getTexture());
	adder.setParameter("bloom", bloom.getTexture());
	applyShader(adder, out);
}
