#include "MusicEmitter.h"
#include "Transform.h"
#include "Entity.h"
#include "Music.h"

#include <ResourcesManager.h>
#include <SoundManager.h>

namespace ECS {
	
	MusicEmitter* MusicEmitter::currentEmitterOnChannel = nullptr;

	MusicEmitter::MusicEmitter(cstring fileName) {
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
	}
	
	void MusicEmitter::changeMusic(cstring soundPath) {
		fileName = soundPath;

		music = Resources::ResourcesManager::instance()->addMusic(fileName);
	}

	void MusicEmitter::start() {

		if (playOnStart)
			play();

	}

	void MusicEmitter::play() {

		if (MusicEmitter::currentEmitterOnChannel != nullptr)
			MusicEmitter::currentEmitterOnChannel->onChannel = false;

		MusicEmitter::currentEmitterOnChannel = this;

		onChannel = true;

		music->setMusicVolume(volume * MIX_MAX_VOLUME);

		music->play(loop);

	}

	void MusicEmitter::pause() {
		if (onChannel)
			music->pause();
	}

	void MusicEmitter::stop() {

		if (onChannel) {
			music->stop();

			onChannel = false;
		}
	}

	void MusicEmitter::resume() {
		if (onChannel)
			music->resume();
	}

	void MusicEmitter::playWithFadeIn(float seconds, int loops) {

		if (MusicEmitter::currentEmitterOnChannel != nullptr)
			MusicEmitter::currentEmitterOnChannel->onChannel = false;

		MusicEmitter::currentEmitterOnChannel = this;
		
		onChannel = true;

		music->setMusicVolume(volume * MIX_MAX_VOLUME);

		music->fadeIn(seconds * 1000.0f, loops);
	}

	void MusicEmitter::fadeOut(float seconds) {
		if (onChannel)
			music->fadeOut(seconds * 1000.0f);
	}

	bool MusicEmitter::isPlaying() {
		if (onChannel)
			return music->isPlaying();

		return false;
	}

	bool MusicEmitter::isPaused() {
		if (onChannel)
			return music->isPaused();

		return false;
	}

	void MusicEmitter::rewind() {
		if (onChannel)
			music->rewindMusic(); 
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

}
