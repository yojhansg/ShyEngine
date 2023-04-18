#include "SoundManager.h"
#include "SDL.h"

namespace Sound {

	SoundManager::SoundManager() {
		initSDLMixer();
	}

	SoundManager::~SoundManager() {
		closeSDLMixer();
	}

	int SoundManager::loadSoundEffect(Mix_Chunk* s) {
		int nSounds = sfxs.size();

		sfxs.push_back(s);

		return nSounds;
	}

	int SoundManager::loadMusic(Mix_Music* m) {
		int nMusic = music.size();

		music.push_back(m);

		return nMusic;
	}

	int SoundManager::playSound(int id, int loop, int channel) {
		return Mix_PlayChannel(channel, sfxs[id], loop);
	}

	void SoundManager::playMusic(int id, int loop) {

		if (musicPlaying()) haltMusic();

		int e = Mix_PlayMusic(music[id], loop);
		assert(e == 0, Mix_GetError());
	}

	int SoundManager::fadeInChannel(int channel, int id, int loops, int ms) {
		return Mix_FadeInChannel(channel, sfxs[id], loops, ms);
	}

	int SoundManager::fadeOutChannel(int channel, int ms) {
		return Mix_FadeOutChannel(channel, ms);
	}

	void SoundManager::fadeInMusic(int id, int loops, int ms) {
		int e = Mix_FadeInMusic(music[id], loops, ms);
		assert(e == 0, Mix_GetError());
	}

	void SoundManager::fadeOutMusic(int ms) {
		Mix_FadeOutMusic(ms);
	}

	void SoundManager::setMasterVolume(float volume) {
		Mix_MasterVolume(volume * MIX_MAX_VOLUME);
	}


	// ------------------ CHANNELS API --------------------------- //

	void SoundManager::pauseChannel(int channel) {

		assert(channel > -1, "Channel must be a positive value.\n");

		Mix_Pause(channel);
	}

	void SoundManager::pauseAllChannels() {
		Mix_Pause(-1);
	}

	bool SoundManager::pausedChannel(int channel) {

		assert(channel > -1, "Channel must be a positive value.\n");

		return Mix_Paused(channel);
	}

	int SoundManager::numberOfPausedChannels() {
		return Mix_Paused(-1);
	}

	void SoundManager::resumeChannel(int channel) {
		Mix_Resume(channel);
	}

	void SoundManager::resumeAllChannels() {
		Mix_Resume(-1);
	}

	bool SoundManager::isChannelPlaying(int channel) {
		return Mix_Playing(channel) != 0;
	}

	int SoundManager::numberOfChannelsPlaying() {
		return Mix_Playing(-1);
	}

	void SoundManager::setChannelVolume(int channel, int volume) {

		assert(volume >= 0 && volume <= 128, "Volume must be a value between 0 and 128\n");

		Mix_Volume(channel, volume);
	}

	int SoundManager::getChannelVolume(int channel) {
		return Mix_Volume(channel, -1);
	}

	void SoundManager::setChannelPosition(int channel, int angle, int distance) {
		Mix_SetPosition(channel, angle, distance);
	}

	void SoundManager::setChannelPanning(int channel, int left, int right) {
		Mix_SetPanning(channel, left, right);
	}


	// --------------------- MUSIC API -----------------------------

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

	bool SoundManager::musicPlaying() {
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
	}

	void SoundManager::closeSDLMixer() {
		Mix_Quit(); 
	}
}