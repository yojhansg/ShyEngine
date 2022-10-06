#pragma once

#include "Singleton.h"
#include <vector>
#include <string>

#include <SDL_mixer.h>

namespace SoundManager {

	class SoundManager : public Singleton<SoundManager> {

		friend Singleton<SoundManager>;

	public:
		~SoundManager();

		void loadSound(const std::string& soundFile);

		void loadMusic(const std::string& musicFile);

		void playSound(int s);

		void playMusic(int m);

		void setVolume(int v);
		
		void togglePlay();

	private:

		SoundManager();

		void initSDLMixer();

		void closeSDLMixer();

		std::vector<Mix_Chunk*> sounds;
		std::vector<Mix_Music*> music;

		int volume;
	};
}