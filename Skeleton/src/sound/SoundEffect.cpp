#include "SoundEffect.h"
#include "SoundManager.h"
#include "SDL_mixer.h"
#include <iostream>

namespace Sound {

	SoundEffect::SoundEffect(const std::string& filepath) {

		soundManager = Sound::SoundManager::instance();

		mix_chunk = Mix_LoadWAV(filepath.c_str());

		std::cout << Mix_GetError() << std::endl;

		assert(mix_chunk != NULL, Mix_GetError());

		sound_id = soundManager->loadSoundEffect(mix_chunk);
	}

	SoundEffect::~SoundEffect() {
		Mix_FreeChunk(mix_chunk);
	}

	int SoundEffect::play(int loop, int channel) {
		return soundManager->playSound(sound_id, loop, channel);
	}

	int SoundEffect::fadeInChannel(int ms, int loops, int channel) {
		return soundManager->fadeInChannel(channel, sound_id, loops, ms);
	}

	int SoundEffect::fadeOutChannel(int ms, int channel) {
		return soundManager->fadeOutChannel(channel, ms);
	}

}
