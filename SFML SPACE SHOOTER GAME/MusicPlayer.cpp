#include <Sound/MusicPlayer.h>


MusicPlayer::MusicPlayer() : mMusic(), mFilenames(), mVolume(50.f) {
	// directory for music 
	mFilenames[Music::MenuMusic] = "Media/Music/MenuMusic.ogg";
	mFilenames[Music::LevelMusic] = "Media/Music/LevelMusic.ogg";
}

void MusicPlayer::play(Music::ID theme) {
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");
	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop() {
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume) {
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused) {
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}
