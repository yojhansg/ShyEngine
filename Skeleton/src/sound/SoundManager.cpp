#include "SoundManager.h"

#include "SDL.h"

namespace SoundManager {

	SoundManager::SoundManager() {
		volume = 0;

		initSDLMixer();
	}

	SoundManager::~SoundManager() {
		closeSDLMixer();
	}

	void SoundManager::loadSound(const std::string& soundFile) {

		Mix_Chunk* s = Mix_LoadWAV(soundFile.c_str());
		assert(s != NULL, Mix_GetError());

		sounds.push_back(s);
	}

	void SoundManager::loadMusic(const std::string& musicFile) {

		Mix_Music* m = Mix_LoadMUS(musicFile.c_str());
		assert(m != NULL, Mix_GetError());

		music.push_back(m);
	}

	void SoundManager::playSound(int s) {
		Mix_Volume(-1, volume);
		int e = Mix_PlayChannel(-1, sounds[s], 0);
		assert(e == 0, Mix_GetError());
	}

	void SoundManager::playMusic(int m) {
		if (Mix_PlayingMusic() == 0) {
			Mix_Volume(1, volume);
			int e = Mix_PlayMusic(music[m], -1 == 0);
			assert(e == 0, Mix_GetError());
		}
	}

	void SoundManager::setVolume(int v) {
		volume = (MIX_MAX_VOLUME * v) / 100;
	}

	void SoundManager::togglePlay() {
		if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
		else Mix_PauseMusic();
	}

	void SoundManager::initSDLMixer() {
		Mix_Init(MIX_INIT_MP3);
		SDL_Init(SDL_INIT_AUDIO);

		int e = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		assert(e >= 0, Mix_GetError());

		setVolume(80);
	}

	void SoundManager::closeSDLMixer() {
		for (int s = 0; s < sounds.size(); s++) {
			Mix_FreeChunk(sounds[s]);
			sounds[s] = NULL;
		}
		for (int m = 0; m < sounds.size(); m++) {
			Mix_FreeMusic(music[m]);
			music[m] = NULL;
		}
		Mix_Quit();
	}
}