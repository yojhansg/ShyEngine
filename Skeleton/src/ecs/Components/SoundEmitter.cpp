#include "SoundEmitter.h"
#include "SoundEffect.h"
#include "Transform.h"
#include "Entity.h"

#include <ResourcesManager.h>

ECS::SoundEmitter::SoundEmitter(cstring fileName) {
	transform = nullptr;
	soundEffect = nullptr;

	loop = false;
	startPlaying = true;

	channel = -1;

	this->fileName = fileName;
}

void ECS::SoundEmitter::init() {

	transform = getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");

	changeSound(fileName);

}

void ECS::SoundEmitter::start() {

	if (startPlaying)
		play();

}

void ECS::SoundEmitter::update(float deltaTime) {

}

void ECS::SoundEmitter::play() {
	channel = soundEffect->play((int) loop);
}

void ECS::SoundEmitter::enableStartPlaying(bool enable) {
	startPlaying = enable;
}

bool ECS::SoundEmitter::startsPlaying() {
	return startPlaying;
}

void ECS::SoundEmitter::changeSound(cstring soundPath) {
	fileName = soundPath;

	soundEffect = Resources::ResourcesManager::instance()->addSound(fileName);
}
