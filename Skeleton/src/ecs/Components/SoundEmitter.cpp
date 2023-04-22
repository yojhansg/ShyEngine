#include "SoundEmitter.h"
#include "SoundEffect.h"
#include "Transform.h"
#include "Entity.h"

#include <ResourcesManager.h>
#include <SoundManager.h>
#include <iostream>

ECS::SoundEmitter::SoundEmitter(cstring fileName) {
	transform = nullptr;
	soundEffect = nullptr;

	loop = false;
	startPlaying = true;
	
	playing = false;
	volume = MIX_MAX_VOLUME;

	channel = -1;

	this->fileName = fileName;
}

void ECS::SoundEmitter::init() {

	transform = getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");

	soundManager = Sound::SoundManager::instance();

	changeSound(fileName);

}

void ECS::SoundEmitter::start() {

	if (startPlaying)
		play();

}

void ECS::SoundEmitter::update(float deltaTime) {

	/*if (!soundManager->isChannelPlaying(channel) && playing) {
		playing = false;
		resetChannel();
	}*/
}

void ECS::SoundEmitter::play() {
	channel = soundEffect->play(loop);

	playing = true;

}

void ECS::SoundEmitter::playWithfadeIn(int ms, int loops) {
	channel = soundEffect->fadeInChannel(ms, loops);

	playing = true;
}

void ECS::SoundEmitter::fadeOut(int ms) {
	channel = soundEffect->fadeOutChannel(ms);
}

void ECS::SoundEmitter::enableStartPlaying(bool enable) {
	startPlaying = enable;
}

bool ECS::SoundEmitter::startsPlaying() {
	return startPlaying;
}

void ECS::SoundEmitter::setLoop(bool loop) {
	this->loop = loop;
}

bool ECS::SoundEmitter::isLoop() {
	return loop;
}

void ECS::SoundEmitter::setVolume(int volume) {
	this->volume = volume;
}

int ECS::SoundEmitter::getVolume() {
	return volume;
}

void ECS::SoundEmitter::resetChannel() {
	//// Pause
	//if (soundManager->pausedChannel(channel))
	//	soundManager->resumeChannel(channel);

	//// Volume
	//soundManager->setChannelVolume(channel, MIX_MAX_VOLUME);

}

void ECS::SoundEmitter::changeSound(cstring soundPath) {
	fileName = soundPath;

	soundEffect = Resources::ResourcesManager::instance()->addSound(fileName);
}
