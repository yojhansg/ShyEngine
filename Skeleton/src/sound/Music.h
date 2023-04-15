#pragma once

#include "SDL_mixer.h"
#include <string>

namespace Sound {

	class SoundManager;

	class Music {

	public:

		Music(const std::string& filepath);

		~Music();

		void play(int loop);

	private:

		SoundManager* soundManager;

		int music_id;

		Mix_Music* mix_music;

	};

}

