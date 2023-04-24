#include "SoundManager.h"
#include "SDL.h"

namespace Sound {

	SoundManager::SoundManager() {
		initSDLMixer();
	}

	SoundManager::~SoundManager() {
		closeSDLMixer();
	}

	void SoundManager::setMasterVolume(float volume) {
		assert(volume >= 0.0f && volume <= 1.0f, "Volume value must be between 0 and 1!");

		Mix_MasterVolume(volume * MIX_MAX_VOLUME);
	}

	void SoundManager::setChannelsCapacity(float nChannels) {
		this->nChannels = nChannels;

		Mix_AllocateChannels(nChannels);
	}


	// ------------------ CHANNELS API --------------------------- //

	int SoundManager::loadSoundEffect(Mix_Chunk* s) {
		int nSounds = sfxs.size();

		sfxs.push_back(s);

		return nSounds;
	}

	int SoundManager::playSound(int id, int loop, int channel) {

		if (channel != -1)
			assert(channel >= 0 && channel < nChannels, "Channel must be between -1 and " + nChannels + ".\n");

		int ret = Mix_PlayChannel(channel, sfxs[id], loop);

		assert(ret != -1, "Sound could not be played\n");

		return ret;
	}

	int SoundManager::fadeInChannel(int channel, int id, int loops, int ms) {

		if (channel != -1)
			assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		int ret = Mix_FadeInChannel(channel, sfxs[id], loops, ms);

		assert(ret != -1, "Sound could not be played\n");

		return ret;
	}

	void SoundManager::fadeOutChannel(int channel, int ms) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		assert(Mix_FadeOutChannel(channel, ms) != -1, Mix_GetError());
	}

	void SoundManager::stopChannel(int channel) {
		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		assert(Mix_HaltChannel(channel) != -1, Mix_GetError());
	}

	void SoundManager::pauseChannel(int channel) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		Mix_Pause(channel);
	}

	bool SoundManager::pausedChannel(int channel) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		return Mix_Paused(channel);
	}

	void SoundManager::resumeChannel(int channel) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		Mix_Resume(channel);
	}

	bool SoundManager::isChannelPlaying(int channel) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		return Mix_Playing(channel) != 0;
	}

	void SoundManager::setChannelVolume(int channel, int volume) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		assert(volume >= 0 && volume < MIX_MAX_VOLUME, "Volume must be a value between 0 and 127 both included\n");

		Mix_Volume(channel, volume);
	}

	int SoundManager::getChannelVolume(int channel) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		return Mix_Volume(channel, -1);
	}

	void SoundManager::setChannelPosition(int channel, int angle, int distance) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		assert(Mix_SetPosition(channel, angle, distance) != 0, Mix_GetError());

	}

	void SoundManager::setChannelPanning(int channel, int left, int right) {

		assert(channel >= 0 && channel < nChannels, "Channel must be between 0 and " + nChannels + ".\n");

		assert(Mix_SetPanning(channel, left, right) != 0, Mix_GetError());

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

		int e = Mix_PlayMusic(music[id], loop);
		assert(e == 0, Mix_GetError());
	}

	void SoundManager::fadeInMusic(int id, int loops, int ms) {

		if (isMusicPlaying())
			haltMusic();

		int e = Mix_FadeInMusic(music[id], loops, ms);
		assert(e == 0, Mix_GetError());
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

	void SoundManager::initSDLMixer() {
		int e = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID);
		assert(e > 0, Mix_GetError());

		SDL_Init(SDL_INIT_AUDIO);

		e = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		assert(e >= 0, Mix_GetError());

		nChannels = 8;

	}

	void SoundManager::closeSDLMixer() {
		Mix_Quit(); 
	}
}