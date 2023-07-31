#include "MusicEmitter.h"
#include "Transform.h"
#include "Entity.h"
#include "Music.h"

#include <ResourcesManager.h>
#include <SoundManager.h>

namespace ECS {
	
	MusicEmitter* MusicEmitter::emitterOnChannel = nullptr;

	MusicEmitter::MusicEmitter(cstring fileName) {

		sManager = Sound::SoundManager::instance();

		transform = nullptr;
		music = nullptr;

		loop = false;
		playOnStart = true;
		onChannel = false;

		volume = .5f;

		this->fileName = fileName;

	}

	MusicEmitter::~MusicEmitter() {
		stop();
	}

	void MusicEmitter::init() {

		transform = getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");

		changeMusic(fileName);

		music_id = music->getMusicId();
	}

	void MusicEmitter::start() {

		if (playOnStart)
			play();

	}
	
	void MusicEmitter::changeMusic(cstring soundPath) {
		fileName = soundPath;

		music = Resources::ResourcesManager::instance()->addMusic(fileName);
	}

	void MusicEmitter::play() {

		if (emitterOnChannel != nullptr)
			emitterOnChannel->onChannel = false;

		emitterOnChannel = this;

		onChannel = true;

		sManager->setMusicVolume(volume * MIX_MAX_VOLUME);

		sManager->playMusic(music_id, loop);

	}

	void MusicEmitter::pause() {
		if (onChannel)
			sManager->pauseMusic();
	}

	void MusicEmitter::stop() {

		if (onChannel) {
			sManager->haltMusic();

			onChannel = false;
		}
	}

	void MusicEmitter::resume() {
		if (onChannel)
			sManager->resumeMusic();
	}

	void MusicEmitter::playWithFadeIn(float seconds, int loops) {

		if (emitterOnChannel != nullptr)
			emitterOnChannel->onChannel = false;

		emitterOnChannel = this;
		
		onChannel = true;

		sManager->setMusicVolume(volume * MIX_MAX_VOLUME);

		sManager->fadeInMusic(music_id, seconds * 1000.0f, loops);
	}

	void MusicEmitter::fadeOut(float seconds) {
		if (onChannel)
			sManager->fadeOutMusic(seconds * 1000.0f);
	}

	bool MusicEmitter::isPlaying() {
		if (onChannel)
			return sManager->isMusicPlaying();

		return false;
	}

	bool MusicEmitter::isPaused() {
		if (onChannel)
			return sManager->musicPaused();

		return false;
	}

	void MusicEmitter::rewind() {
		if (onChannel)
			sManager->rewindMusic();
	}

	void MusicEmitter::setVolume(float volume) {

		assert(volume >= 0.0f && volume <= 1.0f, "Volume value must be between 0 and 1!");

		this->volume = volume;
	}

	float MusicEmitter::getVolume() {
		return volume;
	}

	void MusicEmitter::shouldPlayOnStart(bool play) {
		playOnStart = play;
	}

	void MusicEmitter::setLoop(bool loop) {
		this->loop = loop;
	}

	bool MusicEmitter::isOnLoop() {
		return loop;
	}

}
