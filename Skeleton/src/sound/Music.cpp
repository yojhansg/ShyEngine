#include "Music.h"
#include "SoundManager.h"

#include <ConsoleManager.h>

namespace Sound {

	Music::Music(const std::string& filepath) {

		soundManager = Sound::SoundManager::instance();

		mix_music = Mix_LoadMUS(filepath.c_str());

		if (mix_music == NULL) {
			Console::Output::PrintError("Invalid argument value", Mix_GetError());
			music_id = 0;
			return;
		}

		music_id = soundManager->loadMusic(mix_music);

	}

	Music::~Music() {
		Mix_FreeMusic(mix_music);
	}

	int Music::getMusicId() {
		return music_id;
	}

}
