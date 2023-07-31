#include "SoundEffect.h"
#include "SoundManager.h"
#include "SDL_mixer.h"

namespace Sound {

	SoundEffect::SoundEffect(const std::string& filepath) {

		mix_chunk = Mix_LoadWAV(filepath.c_str());
		assert(mix_chunk != NULL, Mix_GetError());

		sound_id = Sound::SoundManager::instance()->loadSoundEffect(mix_chunk);
	}

	SoundEffect::~SoundEffect() {
		Mix_FreeChunk(mix_chunk);
	}

	int SoundEffect::getSoundId() {
		return sound_id;
	}

}
