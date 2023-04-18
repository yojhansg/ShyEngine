#include "MusicEmitter.h"
#include "Transform.h"
#include "Entity.h"
#include "Music.h"

#include <ResourcesManager.h>

namespace ECS {

	MusicEmitter::MusicEmitter(cstring fileName) {
		transform = nullptr;
		music = nullptr;

		loop = false;
		startPlaying = true;

		this->fileName = fileName;
	}

	void MusicEmitter::init() {

		transform = getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");

		changeMusic(fileName);

	}

	void MusicEmitter::start() {

		if (startPlaying)
			play();

	}

	void MusicEmitter::play() {
		music->play(loop);
	}

	void MusicEmitter::changeMusic(cstring soundPath) {
		fileName = soundPath;

		music = Resources::ResourcesManager::instance()->addMusic(fileName);
	}

	void MusicEmitter::playWithFadeIn(int ms, int loops = -1) {
		music->fadeIn(ms, loops);
	}

	void MusicEmitter::fadeOut(int ms) {
		music->fadeOut(ms);
	}

}
