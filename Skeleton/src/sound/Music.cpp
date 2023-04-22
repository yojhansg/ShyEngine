#include "Music.h"
#include "SoundManager.h"

namespace Sound {

	Music::Music(const std::string& filepath) {

		soundManager = Sound::SoundManager::instance();

		mix_music = Mix_LoadMUS(filepath.c_str());
		assert(mix_music != NULL, Mix_GetError());

		music_id = soundManager->loadMusic(mix_music);

	}

	Music::~Music() {
		Mix_FreeMusic(mix_music);
	}

	void Music::play(int loop) {
		soundManager->playMusic(music_id, loop);
	}

	void Music::pause() {
		soundManager->pauseMusic();
	}

	void Music::resume() {
		soundManager->resumeMusic();
	}

	void Music::stop() {
		soundManager->haltMusic();
	}

	void Music::fadeIn(int ms, int loops) {
		soundManager->fadeInMusic(music_id, loops, ms);
	}

	void Music::fadeOut(int ms) {
		soundManager->fadeOutMusic(ms);
	}

	bool Music::isPlaying() {
		return soundManager->isMusicPlaying();
	}

	bool Music::isPaused() {
		return soundManager->musicPaused();
	}

	void Music::setMusicVolume(float volume) {
		soundManager->setMusicVolume(volume);
	}

	float Music::getMusicVolume() {
		return soundManager->getMusicVolume();
	}

	void Music::rewindMusic() {
		soundManager->rewindMusic();
	}

}
