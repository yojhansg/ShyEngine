#include "MusicEmitter.h"
#include "Transform.h"
#include "Entity.h"

#include <ResourcesManager.h>
#include <SoundManager.h>

ECS::MusicEmitter::MusicEmitter(cstring fileName) {
	transform = nullptr;
	music = nullptr;

	this->fileName = fileName;
}

void ECS::MusicEmitter::init() {

	transform = getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");

	changeMusic(fileName);

}

void ECS::MusicEmitter::changeMusic(cstring soundPath) {
	fileName = soundPath;

	music = Resources::ResourcesManager::instance()->addMusic(fileName);
}
