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

}
