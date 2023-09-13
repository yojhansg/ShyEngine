#include "MusicEmitter.h"
#include "Transform.h"
#include "Entity.h"
#include "Music.h"

#include <ResourcesManager.h>
#include <ConsoleManager.h>
#include <SoundManager.h>

namespace ECS {
	
	MusicEmitter* MusicEmitter::emitterOnChannel = nullptr;

	MusicEmitter::MusicEmitter() {

		sManager = Sound::SoundManager::instance();

		transform = nullptr;
		music = nullptr;

		loop = false;
		playOnStart = true;
		onChannel = false;

		volume = .5f;

		this->fileName = "";

	}

	MusicEmitter::~MusicEmitter() {
		stop();
	}

	void MusicEmitter::init() {

		transform = getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		loadMusic(fileName);

		music_id = music->getMusicId();
	}

	void MusicEmitter::start() {

		if (playOnStart)
			play();

	}
	
	void MusicEmitter::loadMusic(cstring soundPath) {
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

		if (volume < 0.0f || volume > 1.0f) {
			Console::Output::PrintError("Invalid volume argument", "Volume value must be between 0 and 1!");
			return;
		}

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
