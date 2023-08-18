#include "SoundManager.h"
#include "SDL.h"

#include <ConsoleManager.h>

#define MAX_CHANNELS_CAPACITY 32
#define MIN_CHANNELS_CAPACITY 4

namespace Sound {

	SoundManager::SoundManager() {
		valid = initSDLMixer();
	}

	bool SoundManager::Valid() {
		return valid;
	}

	SoundManager::~SoundManager() {
		closeSDLMixer();
	}

	void SoundManager::setMasterVolume(float volume) {
		if (volume < 0.0f || volume > 1.0f)
			Console::Output::PrintWarning("Invalid argument value", "Volume value must be between 0 and 1!");

		Mix_MasterVolume(volume * MIX_MAX_VOLUME);
	}

	void SoundManager::setChannelsCapacity(float nChannels) {

		if (nChannels < MIN_CHANNELS_CAPACITY || nChannels > MAX_CHANNELS_CAPACITY)
			Console::Output::PrintError("Invalid argument value", "The number of channels can not be greater than 32 or less than 4!");
		else {
			this->nChannels = nChannels;
			Mix_AllocateChannels(nChannels);
		}
	}


	// ------------------ CHANNELS API --------------------------- //

	int SoundManager::loadSoundEffect(Mix_Chunk* s) {
		int nSounds = sfxs.size();

		sfxs.push_back(s);

		return nSounds;
	}

	int SoundManager::playSound(int id, int loop, int channel) {

		if (channel < -1 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between -1 and " + nChannels - 1);
			return -1;
		}

		int ret = Mix_PlayChannel(channel, sfxs[id], loop);

		if (ret == -1)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());

		return ret;
	}

	int SoundManager::fadeInChannel(int channel, int id, int loops, int ms) {

		if (channel < -1 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between -1 and " + nChannels - 1);
			return -1;
		}

		int ret = Mix_FadeInChannel(channel, sfxs[id], loops, ms);

		if (ret == -1)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());

		return ret;
	}

	void SoundManager::fadeOutChannel(int channel, int ms) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		if (Mix_FadeOutChannel(channel, ms) == -1)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());
	}

	void SoundManager::stopChannel(int channel) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		if (Mix_HaltChannel(channel) == -1)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());
	}

	void SoundManager::pauseChannel(int channel) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		Mix_Pause(channel);
	}

	bool SoundManager::pausedChannel(int channel) {

		if (channel < 0 || channel >= nChannels)
			Console::Output::PrintWarning("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);

		return Mix_Paused(channel);
	}

	void SoundManager::resumeChannel(int channel) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		Mix_Resume(channel);
	}

	bool SoundManager::isChannelPlaying(int channel) {

		if (channel < 0 || channel >= nChannels)
			Console::Output::PrintWarning("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);

		return Mix_Playing(channel) != 0;
	}

	void SoundManager::setChannelVolume(int channel, int volume) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		if (volume < 0 || volume >= MIX_MAX_VOLUME)
			Console::Output::PrintWarning("Invalid argument value", "Volume value must be between 0 and " + MIX_MAX_VOLUME - 1);

		Mix_Volume(channel, volume);
	}

	int SoundManager::getChannelVolume(int channel) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return 0;
		}

		return Mix_Volume(channel, -1);
	}

	void SoundManager::setChannelPosition(int channel, int angle, int distance) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		if (Mix_SetPosition(channel, angle, distance) == 0)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());

	}

	void SoundManager::setChannelPanning(int channel, int left, int right) {

		if (channel < 0 || channel >= nChannels) {
			Console::Output::PrintError("Invalid argument value", "Channel value must be between 0 and " + nChannels - 1);
			return;
		}

		if (Mix_SetPanning(channel, left, right) == 0)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());

	}


	// --------------------- MUSIC API -----------------------------

	int SoundManager::loadMusic(Mix_Music* m) {
		int nMusic = music.size();

		music.push_back(m);

		return nMusic;
	}

	void SoundManager::playMusic(int id, int loop) {

		if (isMusicPlaying())
			haltMusic();

		if (Mix_PlayMusic(music[id], loop) == -1)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());
	}

	void SoundManager::fadeInMusic(int id, int loops, int ms) {

		if (isMusicPlaying())
			haltMusic();

		if (Mix_FadeInMusic(music[id], loops, ms) == -1)
			Console::Output::PrintError("Sound engine (SDL_Mixer)", Mix_GetError());
	}

	void SoundManager::fadeOutMusic(int ms) {
		Mix_FadeOutMusic(ms);
	}

	void SoundManager::pauseMusic() {
		Mix_PauseMusic();
	}

	bool SoundManager::musicPaused() {
		return Mix_PausedMusic();
	}

	void SoundManager::haltMusic() {
		Mix_HaltMusic();
	}

	void SoundManager::resumeMusic() {
		Mix_ResumeMusic();
	}

	bool SoundManager::isMusicPlaying() {
		return Mix_PlayingMusic() != 0;
	}

	void SoundManager::setMusicVolume(int volume) {
		Mix_VolumeMusic(volume);
	}

	int SoundManager::getMusicVolume() {
		return Mix_VolumeMusic(-1);
	}

	void SoundManager::rewindMusic() {
		Mix_RewindMusic();
	}

	bool SoundManager::initSDLMixer() {

		int flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID;

		int e = Mix_Init(flags);

		// Checks for errors in the SDL_Mixer initialisation
		if ((e & flags) != flags) {
			Console::Output::PrintError("Sound engine (SDL_Mixer)", "Could not initialise SDLMixer!");
			Mix_Quit();
			return false;
		}

		e = SDL_Init(SDL_INIT_AUDIO);

		// Checks for errors in the SDL_Init method
		if (e < 0) {
			Console::Output::PrintError("Sound engine (SDL_Mixer)", SDL_GetError());
			Mix_Quit();
			return false;
		}

		e = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		if (e == -1) {
			Console::Output::PrintError("Sound engine (SDL_Mixer)", "Could not initialise SDLMixer!");
			Mix_Quit();
			return false;
		}

		nChannels = 8;

		return true;
	}

	void SoundManager::closeSDLMixer() {
		Mix_Quit(); 
	}

}