#pragma once

#include <string>

struct Mix_Chunk;

namespace Sound {

	class SoundManager;

	class SoundEffect {

	public:

		SoundEffect(const std::string& filepath);

		~SoundEffect();

		int play(int loop, int channel = -1);

	private:

		SoundManager* soundManager;

		int sound_id;

		Mix_Chunk* mix_chunk;
	};

}

