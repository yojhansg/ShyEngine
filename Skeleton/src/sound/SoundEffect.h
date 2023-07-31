#pragma once

#include <string>

struct Mix_Chunk;

namespace Sound {

	class SoundManager;

	class SoundEffect {

	public:

		SoundEffect(const std::string& filepath);

		~SoundEffect();

		int getSoundId();

	private:

		int sound_id;

		Mix_Chunk* mix_chunk;
	};

}

