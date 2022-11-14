#include "SoundManager.h"
#include "SDL.h"

namespace SoundManager {

	SoundManager::SoundManager() {
		initSDLMixer();
	}

	SoundManager::~SoundManager() {
		closeSDLMixer();
	}

	void SoundManager::load(const std::string& fileName, const std::string& id, SOUND_TYPE type)
	{
		if (type == SOUND_MUSIC)
		{
			Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());
			assert(pMusic != NULL, Mix_GetError());

			music[id] = pMusic;
		}
		else if (type == SOUND_SFX)
		{
			Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());
			assert(pChunk != NULL, Mix_GetError());

			sfxs[id] = pChunk;
		}
	}

	int SoundManager::playSound(const std::string& id, int loop, int channel) {

		return Mix_PlayChannel(channel, sfxs[id], loop);
	}

	void SoundManager::playMusic(const std::string& id, int loop) {

		if (musicPlaying()) haltMusic();

		int e = Mix_PlayMusic(music[id], loop);
		assert(e == 0, Mix_GetError());
	}

	void SoundManager::playWithFadeIn(int channel, const std::string& id, int loops, int ms) {
		Mix_FadeInChannel(channel, sfxs.at(id), loops, ms);
	}

	void SoundManager::playMusicWithFadeIn(const std::string& id, int loops, int ms) {
		int e = Mix_FadeInMusic(music.at(id), loops, ms);
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

		return  Mix_Volume(channel, -1);
	}

	void SoundManager::setChannelPosition(int channel, Sint16 angle, Uint8 distance) {
		Mix_SetPosition(channel, angle, distance);
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

	void SoundManager::rewingMusic() {
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

		for (auto &s : sfxs)
			Mix_FreeChunk(s.second);

		for (auto &m : music) 
			Mix_FreeMusic(m.second);

		Mix_Quit(); 
	}
}